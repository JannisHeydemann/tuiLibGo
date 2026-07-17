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

// Version returns the ctui library version string.
//
// This is the example wrapper matching ctui_version() in bridge.h — replace
// it with real wrappers as you grow the C API. Pattern for a returned C
// string you own: defer C.free(unsafe.Pointer(p)) after C.GoString(p).
func Version() string {
	return C.GoString(C.ctui_version())
}
