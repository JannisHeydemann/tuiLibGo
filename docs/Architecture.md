# Architecture

tuiLib is split into four layers. Each layer only talks to its immediate
neighbor, which keeps the "real" C++ types out of Go entirely.

```
demo/main.go            (Go program)
      │  import "tuilib/goTui"
      ▼
goTui/ctui.go            (Go package, cgo)
      │  #cgo LDFLAGS: -L.../ctui/build -lctui
      │  calls into C functions declared in bridge.h
      ▼
ctui/bridge.h + bridge.cpp   (extern "C" shim)
      │  thin wrappers, no C++ types cross this line
      ▼
ctui/font/*, ctui/mainMenu/*   (real C++ implementation)
```

## Why the bridge layer exists

cgo (Go's C interop) only understands C, not C++. It cannot see
`std::string`, classes, templates, or overloads. So every piece of C++
functionality that Go needs has to be re-exposed as a plain C function
declared inside `extern "C" { ... }` in `ctui/bridge.h`, implemented in
`ctui/bridge.cpp` as a thin shim over the real C++ code.

Rules the bridge follows (see the comments at the top of `bridge.h`/`bridge.cpp`):

- **Opaque handles only.** No C++ type (std::string, a class, a template
  instantiation) may appear in a function signature declared inside
  `extern "C"`. Structs crossing the boundary (like `CtuiFontLines`) use
  only C-compatible members (`char**`, `int`, etc.).
- **Manual memory ownership.** Anything allocated on the C++ side with
  `new`/`malloc`/`strdup` must be freed through a C function the bridge
  exposes (e.g. `ctui_font_free`) — Go's garbage collector has no visibility
  into the C++ heap and cannot free it for you.
- **No C++ exceptions cross into Go.** A C++ exception unwinding into Go
  code is undefined behavior; any bridge function that could throw must
  catch internally and convert to an error code or out-parameter instead.
  (Today's bridge functions don't throw, but keep this in mind when adding
  new ones.)

See [CGo Bridge](CGo-Bridge.md) for the concrete steps to add a new function
across this boundary.

## Build-time wiring

- `ctui/Makefile` compiles every `.cpp` under `ctui/` (except `main.cpp`,
  the standalone test harness) into `ctui/build/libctui.a`.
- `goTui/ctui.go` links against that static archive via cgo directives:
  ```go
  #cgo CFLAGS: -I${SRCDIR}/../ctui
  #cgo LDFLAGS: -L${SRCDIR}/../ctui/build -lctui -lstdc++
  #include "bridge.h"
  ```
  `-lstdc++` is required because libctui.a itself is C++ under the hood.
- `demo/` is a separate Go module (own `go.mod`) that depends on the root
  module `tuilib` via a `replace tuilib => ../` directive, so it always
  builds against the local `goTui` package rather than a published version.

See [Building and Running](Building-and-Running.md) for the full command
reference, and [Project Status](Project-Status.md) for what's implemented
vs. still a stub (notably `ctui::mainMenu`).
