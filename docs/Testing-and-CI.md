# Testing and CI

## C++ tests (GoogleTest)

Location: `ctui/tests/font_test.cpp` (+ empty placeholder `font_test.hpp`).

Run with:

```sh
make -C ctui run     # or: cd ctui && make run
```

This builds `ctui/build/ctui_test` (GoogleTest sources + all library
object files, but not `main.o`) and executes it. See
[Building and Running](Building-and-Running.md) for how the Makefile wires
this together.

### What's covered today

All current tests target `generateFont()` (see
[Font Module](Font-Module.md)):

| Test | Checks |
|------|--------|
| `FontTest.Dimensions` | Output has `LINECOUNT` rows, each with one column per input letter, and each cell is `FONTSIZE` (+`SPACECOUNT` if not the last letter) characters wide. |
| `FontTest.RendersKnownGlyph` | Rendering `"A"` matches the `AsciiLetters['A']` grid, row by row, converted with the chosen `fontChar`. |
| `FontTest.LowercaseIsUppercased` | Rendering `"a"` and `"A"` produce identical output. |
| `FontTest.UsesFontCharNotHardcoded` | The custom `fontChar` (`'@'`) appears in the output, and a different char (`'#'`) never leaks in — guards against accidentally hardcoding a pen character. |
| `FontTest.MultipleLetters` | Multi-letter input produces one column per letter, on every line. |

There is no test coverage yet for `ctui::mainMenu` (it's an unimplemented
stub — see [Project Status](Project-Status.md)) or for the cgo bridge layer
(`bridge.cpp`) or the Go wrapper (`goTui/ctui.go`) — those would need a Go
test (`go test`) exercising `RenderFont`/`Version`, since GoogleTest can't
reach across the cgo boundary.

### Adding a test

Add a new `TEST(...)` block to `ctui/tests/font_test.cpp`, or a new file
under `ctui/tests/*.cpp` — the Makefile picks up any `tests/*.cpp` file
automatically (`GTEST_SRCS := $(wildcard tests/*.cpp)`), no Makefile edit
required.

## CI

`.gitlab-ci.yml` defines a single stage/job:

```yaml
stages:
  - test

cpp-tests:
  stage: test
  image: ubuntu:24.04
  before_script:
    - apt-get update -qq
    - apt-get install -y -qq --no-install-recommends g++ make libgtest-dev
  script:
    - make -C ctui run
```

It installs `g++`, `make`, and `libgtest-dev` on a fresh `ubuntu:24.04`
image, then runs the exact same `make -C ctui run` command described above.

**Not currently covered by CI:** the Go side (`goTui`, `demo`) is never
built or tested in the pipeline — only the C++ test suite runs. If you add
Go tests, you'll need a new CI job (or stage) that also builds
`libctui.a` first via `make lib`, since `goTui` requires it to link.
