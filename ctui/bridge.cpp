// bridge.cpp — implements the C API from bridge.h as a shim over C++.
// Rules that bite:
//   * Anything you `new` here must be freed via a C free function you expose
//     (Go's GC can't see the C++ heap).
//   * Never let a C++ exception escape into Go — catch it here and convert
//     to an error code / out-param.
#include "bridge.h"

// #include "mainMenu/mainMenu.hpp"

extern "C" {

const char *ctui_version(void) {
    return "ctui 0.1.0";
}

}
