// Package ctui is a Go wrapper around the C++ ctui library.
//
// Build the C library first (from ../ctui):  make lib
// Then anything importing this package links against build/libctui.a.
package ctui

/*
#cgo CFLAGS: -I${SRCDIR}/../ctui
#cgo LDFLAGS: -L${SRCDIR}/../ctui/build -lctui -lstdc++
#include "bridge.h"
#include <stdlib.h>
*/
import "C"
import "unsafe"

// Version returns the ctui library version string.
//
// This is the example wrapper matching ctui_version() in bridge.h — replace
// it with real wrappers as you grow the C API. Pattern for a returned C
// string you own: defer C.free(unsafe.Pointer(p)) after C.GoString(p).
func Version() string {
	return C.GoString(C.ctui_version())
}

// RenderFont renders text as a block/FIGlet-style banner, one string per
// output line (see generateFont in ctui/font/font.hpp for the exact
// rendering rules). fontChar is the character used to draw "on" pixels;
// everything else is rendered as spaces. Letters are matched
// case-insensitively; characters with no glyph render as blank columns.
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
