# Project Status

A snapshot of what's implemented vs. stubbed out, as of commit `606edff`.
Update this page as things move from one column to the other.

## Implemented

- **Font/banner rendering** (`ctui/font/`) — fully working, tested. See
  [Font Module](Font-Module.md). Supports `A`–`Z` and the symbols
  `! = ? % & $ # + * ~`.
- **CGo bridge** (`ctui/bridge.h`/`.cpp`) — working for the two functions it
  exposes today (`ctui_version`, `ctui_font_render`/`ctui_font_free`). See
  [CGo Bridge](CGo-Bridge.md).
- **Go wrapper** (`goTui/ctui.go`) — `Version()` and `RenderFont()`, both
  exercised by `demo/main.go`.
- **Build tooling** — top-level and `ctui/` Makefiles, GitLab CI running the
  C++ test suite. See [Building and Running](Building-and-Running.md) and
  [Testing and CI](Testing-and-CI.md).

## Stubbed / not yet implemented

- **`ctui::mainMenu`** (`ctui/mainMenu/mainMenu.hpp`/`.cpp`) — holds only
  private state (`title`, `options`); no public API, no rendering, no input
  handling, and its `.cpp` file is just a `// TODO: implement your MainMenu
  here.` comment. Not wired into the bridge or Go side yet — the include is
  commented out in `bridge.cpp`.
- **`ctui/main.cpp`** — standalone harness with a `// TODO: exercise your
  library here as you build it` comment; currently just prints a banner
  line and returns.
- **Digits and extended punctuation** in the font glyph table — only
  letters and a handful of symbols are defined (see
  [Font Module](Font-Module.md#currently-supported-characters)).
- **Go-side tests** — no `go test` coverage for `goTui` or `demo`; CI only
  runs the C++ suite (see [Testing and CI](Testing-and-CI.md)).
- **Error handling across the bridge** — none of today's bridge functions
  can fail, so there's no established error-code/out-parameter convention
  yet. Worth deciding before adding a bridge function that *can* fail (e.g.
  invalid UTF-8 input, allocation failure).

## Known rough edges

- `font.hpp`/`font.cpp` used the parameter name `size` in one file and
  `textLength` in the other before this documentation pass unified the
  header's declaration to `textLength` — purely cosmetic (parameter names
  in a declaration don't affect linkage), but worth keeping in sync if you
  touch either file.
- `CTUI_VERSION` in `bridge.cpp` is a `#define` with a trailing semicolon
  inside the macro body (`"0.1.0";`) — harmless today since it's only used
  once as a `return` expression, but easy to trip over if reused elsewhere.

## Suggested next steps

Pick based on what you're actually trying to build next; this is not a
committed roadmap.

1. Implement `ctui::mainMenu` (rendering + option navigation), then bridge
   it following the steps in [CGo Bridge](CGo-Bridge.md).
2. Add digits/more punctuation to `AsciiLetters` if banners need to render
   numbers.
3. Add a `go test` suite for `goTui`, and a CI job that builds `libctui.a`
   before running it.
