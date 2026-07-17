# tuiLib Wiki

**tuiLib** (import path `tuilib`, C++ library name `ctui`) is a terminal UI
library with its core written in C++ and exposed to Go through a small cgo
bridge. The idea: build the actual widgets/rendering logic once in C++,
then let Go programs (like the `demo` module) consume it as a normal Go
package.

This wiki documents the current state of the code as of commit `606edff`
("started work on the generateFont() function...").

## Pages

- [Architecture](Architecture.md) — how the pieces (C++ core, cgo bridge, Go
  wrapper, demo) fit together and why the boundary is shaped the way it is.
- [Font Module](Font-Module.md) — the block/FIGlet-style text renderer,
  currently the only implemented feature.
- [CGo Bridge](CGo-Bridge.md) — the C API surface in `ctui/bridge.h` and how
  to extend it safely when adding new C++ functionality.
- [Building and Running](Building-and-Running.md) — Makefile targets, how to
  build the static library, run the demo, and run the test suite.
- [Testing and CI](Testing-and-CI.md) — the GoogleTest suite and the GitLab
  CI pipeline that runs it.
- [Project Status](Project-Status.md) — what's implemented, what's a stub,
  and known gaps/TODOs.

## Quick start

```sh
make demo   # builds ctui/build/libctui.a, then runs demo/main.go
make run    # alias for `make demo`
make -C ctui run   # builds and runs the C++ GoogleTest suite
```

See [Building and Running](Building-and-Running.md) for details.

## Repository layout

```
.
├── ctui/            # C++ core library ("ctui")
│   ├── bridge.h/.cpp    # C API surface consumed by Go via cgo
│   ├── font/            # block/FIGlet-style text rendering
│   ├── mainMenu/        # WIP menu widget (stub, unimplemented)
│   ├── tests/           # GoogleTest suite
│   ├── main.cpp         # standalone C++ test harness (not in libctui.a)
│   └── Makefile         # builds libctui.a and the test binary
├── goTui/           # Go package wrapping ctui via cgo (import "tuilib/goTui")
├── demo/            # standalone Go module exercising goTui
├── Makefile         # top-level orchestration (lib/demo/run/clean)
└── .gitlab-ci.yml   # CI: builds and runs the C++ test suite
```
