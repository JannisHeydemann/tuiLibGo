# CGo Bridge

Location: `ctui/bridge.h`, `ctui/bridge.cpp`, consumed from `goTui/ctui.go`.

This is the C-compatible seam between the real C++ implementation
(`ctui/font/`, `ctui/mainMenu/`, ...) and Go. See
[Architecture](Architecture.md) for why this layer needs to exist at all;
this page is the practical "how do I add to it" reference.

## Current API surface

```c
const char *ctui_version(void);

typedef struct {
    char **lines;      // array of NUL-terminated C strings, one per row
    int lineCount;
} CtuiFontLines;

CtuiFontLines ctui_font_render(const char *text, char fontChar);
void ctui_font_free(CtuiFontLines lines);
```

| C function         | Wraps                          | Go wrapper (`goTui/ctui.go`) |
|---------------------|---------------------------------|------------------------------|
| `ctui_version`      | `#define CTUI_VERSION` literal | `ctui.Version() string`      |
| `ctui_font_render`  | `generateFont()` (font.hpp)     | `ctui.RenderFont(text string, fontChar byte) []string` |
| `ctui_font_free`    | frees a `CtuiFontLines`         | called internally by `RenderFont` via `defer` |

## Memory ownership rules

- `ctui_font_render` allocates the `char**` array with `malloc` and each row
  with `strdup`. **The caller owns the result** and must pass it to
  `ctui_font_free` exactly once — Go's GC cannot see or free C++/C heap
  memory, so `goTui/ctui.go` always pairs the call with
  `defer C.ctui_font_free(result)`.
- `ctui_version` returns a pointer to a static string literal (`CTUI_VERSION`)
  — nothing to free.

## Adding a new bridge function

Say you've implemented something new in C++ (e.g. wiring up
`ctui::mainMenu`) and want to call it from Go:

1. **Declare** a plain-C function signature in `ctui/bridge.h`, inside the
   existing `extern "C" { ... }` block. Only C-compatible types may appear
   in the signature: primitives, `char*`/`char**`, and plain structs made of
   the same (see `CtuiFontLines` as the template). Never put a C++ class,
   `std::string`, or template type directly in the signature.
2. **Implement** it in `ctui/bridge.cpp` as a thin shim: convert C
   arguments to C++ types, call the real C++ API (e.g. something in
   `mainMenu/mainMenu.hpp`), convert the result back to C-compatible output,
   and return.
   - Any C++-side allocation destined to outlive the call must be copied
     onto the C heap (`malloc`/`strdup`, as `ctui_font_render` does) —
     never return a raw pointer into a `std::string` or `std::vector`.
   - Wrap any code that could throw in `try { ... } catch (...) { ... }` and
     convert to an error code / out-parameter. No exception may propagate
     out of an `extern "C"` function.
   - If the new function allocates anything, add a matching `ctui_*_free`
     function and document the ownership contract in a comment, mirroring
     `ctui_font_free`.
3. **Rebuild** the static library: `make -C ctui lib` (or `make lib` from
   the repo root).
4. **Wrap** it in `goTui/ctui.go`: convert Go arguments to C types
   (`C.CString`, `C.int`, ...), call the bridge function, convert results
   back to Go types, and free any C-owned memory with `defer` before
   returning — following the exact pattern `RenderFont` uses today.
5. **Use** it from `demo/main.go` or a test to confirm the whole chain
   (C++ → bridge → cgo → Go) links and runs.

## Gotchas specific to this codebase

- `ctui/Makefile` builds every `.cpp` under `ctui/` **except** `main.cpp`
  into `libctui.a` (see the `LIB_SRCS` filter). If you add a new `.cpp`
  file, it's picked up automatically by the `wildcard` — no Makefile edit
  needed, as long as it isn't named `main.cpp`.
- `goTui/ctui.go`'s `#cgo LDFLAGS` points at `ctui/build/libctui.a`, so the
  C++ library must be rebuilt (`make lib`) before Go picks up bridge
  changes — cgo does not rebuild the C++ side for you.
- `mainMenu/mainMenu.hpp` is currently commented out of `bridge.cpp`'s
  includes (`// #include "mainMenu/mainMenu.hpp"`) since there's nothing to
  bridge yet — see [Project Status](Project-Status.md).
