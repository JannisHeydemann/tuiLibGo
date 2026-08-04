// Package ctui is a Go wrapper around the C++ ctui library.
//
// Build the C library first: make lib
// Then anything importing this package links against build/libctui.a.
package ctui

/*
#cgo CFLAGS: -I${SRCDIR}/../src -I${SRCDIR}/../src/bridge
#cgo LDFLAGS: -L${SRCDIR}/../build -lctui -lstdc++
#include "bridge.h"
#include <stdlib.h>
*/
import "C"
import (
	"errors"
	"unsafe"
)

// Version returns the ctui library version string.
func Version() string {
	return C.GoString(C.ctui_version())
}

// RenderFont renders text as a block/FIGlet-style banner, one string per
// output line. fontChar is the character used to draw "on" pixels;
// everything else is rendered as spaces.
func RenderFont(text string, fontChar byte) []string {
	cText := C.CString(text)
	defer C.free(unsafe.Pointer(cText))

	result := C.ctui_font_render(cText, C.char(fontChar))
	defer C.ctui_font_free(result)

	cLines := unsafe.Slice(result.lines, int(result.lineCount))
	lines := make([]string, result.lineCount)
	for i, cLine := range cLines {
		lines[i] = C.GoString(cLine)
	}
	return lines
}

// =========================================================================
// Engine Wrapper
// =========================================================================

// Engine wraps the underlying C++ TUI engine instance.
type Engine struct {
	handle C.CTuiEngine
}

// NewEngine initializes terminal raw mode, creates the alternate screen buffer,
// and sets up the canvas grid with the specified width and height.
func NewEngine(width, height int) (*Engine, error) {
	handle := C.ctui_engine_create(C.int(width), C.int(height))
	if handle == nil {
		return nil, errors.New("failed to initialize C++ TUI engine")
	}
	return &Engine{handle: handle}, nil
}

// Close destroys the underlying C++ engine and restores the user's terminal back
// to standard canonical mode. Must be called when finished (e.g. using defer).
func (e *Engine) Close() {
	if e.handle != nil {
		C.ctui_engine_destroy(e.handle)
		e.handle = nil
	}
}

// Clear resets the back-buffer canvas back to empty space characters.
func (e *Engine) Clear() {
	if e.handle == nil {
		return
	}
	C.ctui_engine_clear(e.handle)
}

// DrawText writes a string to the back-buffer starting at grid position (x, y).
func (e *Engine) DrawText(x, y int, text string) {
	if e.handle == nil {
		return
	}
	cText := C.CString(text)
	defer C.free(unsafe.Pointer(cText))
	C.ctui_engine_draw_text(e.handle, C.int(x), C.int(y), cText)
}

// Render flushes the frame buffer out to stdout in a single write call.
func (e *Engine) Render() {
	if e.handle == nil {
		return
	}
	C.ctui_engine_render(e.handle)
}
