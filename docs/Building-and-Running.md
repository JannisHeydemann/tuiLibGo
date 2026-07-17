# Building and Running

## Prerequisites

- `g++` supporting C++17 (`ctui/Makefile` builds with `-std=c++17`)
- `make`, `ar`
- Go 1.22+ (see `go.mod` / `demo/go.mod`)
- GoogleTest (`libgtest-dev`) — only needed to build/run the C++ test suite
- `clangd` users: `.clangd` at the repo root adds `-std=c++20` and a
  hardcoded `--gcc-install-dir`; adjust the latter if your toolchain path
  differs.

## Top-level Makefile

Run from the repo root:

| Target      | Effect |
|-------------|--------|
| `make lib`  | Builds `ctui/build/libctui.a` (delegates to `ctui/Makefile`'s `lib` target). |
| `make demo` | Depends on `lib`, then runs `cd demo && go run .` |
| `make run`  | Alias for `make demo`. |
| `make clean`| Delegates to `ctui/Makefile`'s `clean` (removes `ctui/build/`). |

```sh
make demo
# builds libctui.a, then runs demo/main.go, which prints the version
# and a rendered banner for "Jannis!"
```

## `ctui/Makefile` (C++ side)

Run from `ctui/`, or via `make -C ctui <target>` from the root:

| Target      | Effect |
|-------------|--------|
| `lib`       | Compiles every `.cpp` under `ctui/` **except `main.cpp`** into `build/libctui.a` (a static archive via `ar rcs`). This is what the Go wrapper links against. |
| `test`      | Compiles `tests/*.cpp` (GoogleTest sources) together with all library objects (not `main.o`) into `build/ctui_test`. |
| `run`       | Depends on `test`, then executes `build/ctui_test`. |
| `all`       | Alias for `lib`. |
| `clean`     | Removes `build/`. |

```sh
cd ctui
make run     # build + execute the GoogleTest suite
```

Object files mirror the source tree under `build/` (e.g.
`ctui/font/font.cpp` → `ctui/build/font/font.o`), via the generic
`$(BUILD_DIR)/%.o: %.cpp` rule.

### Why `main.cpp` is excluded from the library

`ctui/main.cpp` is a standalone C++ test harness for poking at the library
directly (see comments in that file) — it defines its own `main()`, which
would conflict with GoogleTest's `main()` (from `-lgtest_main`) if linked
into `ctui_test`, and has no place in a *library* archive either. The
Makefile's `LIB_SRCS` filters it out explicitly:

```make
LIB_SRCS := $(filter-out main.cpp,$(wildcard *.cpp) $(wildcard */*.cpp))
```

There is currently no Makefile target that builds and runs `main.cpp`
itself; do so manually if needed, e.g.:

```sh
g++ -std=c++17 -Ictui main.cpp ctui/font/font.cpp -o /tmp/ctui_harness
```

## Go modules

- Root module `tuilib` (`go.mod`) contains `goTui/`, the cgo wrapper package.
- `demo/` is a **separate** Go module (`tuilib-demo`, own `go.mod`) that
  requires `tuilib` but redirects it to the local checkout via
  `replace tuilib => ../`. This means `demo` always builds against your
  local `goTui` changes, never a published version.
- Because `goTui/ctui.go` uses cgo, `CGO_ENABLED=1` (the Go default on most
  platforms) is required, and `ctui/build/libctui.a` must exist before
  compiling/running any Go code that imports `tuilib/goTui` — that's why
  every Go-facing Makefile target depends on `lib` first.

## Editor / tooling integration

- `.zed/tasks.json` defines a "Run Tests" task equivalent to
  `cd ctui && make clean && make run`.
- `compile_commands.json` (gitignored) is expected to be generated via
  `bear -- make` if you want accurate clangd indexing of the C++ build.
