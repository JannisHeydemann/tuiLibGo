# Font Module

Location: `ctui/font/font.hpp`, `ctui/font/font.cpp`.

This is the only fully implemented feature in the library today: rendering
text as a block/FIGlet-style banner, where each character is drawn as a
5-wide by 5-tall grid of a chosen "pen" character.

## Public API

```cpp
void generateFont(std::string text, int textLength, char fontChar,
                   std::vector<std::vector<std::string>>& outVec);
void fillHashMap();
```

### `generateFont(text, textLength, fontChar, outVec)`

Renders `text` into `outVec`, overwriting whatever it previously held.

- `outVec` ends up sized `LINECOUNT` rows × `textLength` columns.
- `outVec[line][letter]` is the fully-rendered string for one glyph's row:
  `FONTSIZE` characters of `fontChar`/space, plus `SPACECOUNT` trailing
  spaces (skipped after the last letter) to separate it from the next glyph.
- Letters are matched case-insensitively — `text` is uppercased internally
  before lookup.
- A character with no entry in `AsciiLetters` renders as blank columns
  rather than erroring (accessing a missing key via `operator[]` on the map
  default-constructs an all-`false` row).
- `fillHashMap()` is called lazily, exactly once across all calls, via a
  function-local `static bool initialized` guard.

To print a banner, concatenate each row's columns and print `LINECOUNT`
lines:

```cpp
std::vector<std::vector<std::string>> out;
generateFont("HI", 2, '#', out);
for (auto& row : out) {
    for (auto& cell : row) std::cout << cell;
    std::cout << '\n';
}
```

(This concat-then-print pattern is exactly what `ctui_font_render` in
`bridge.cpp` does before handing rows to Go — see [CGo Bridge](CGo-Bridge.md).)

### `fillHashMap()`

Populates the module-level `AsciiLetters` map with one entry per supported
character. Idempotent (safe to call again; it just re-assigns the same
entries). You will only need to call it directly in tests — normal callers
go through `generateFont`.

## The glyph table: `AsciiLetters`

```cpp
extern std::unordered_map<char, std::array<std::array<bool, FONTSIZE>, LINECOUNT>> AsciiLetters;
```

Each entry is a `LINECOUNT` (5) × `FONTSIZE` (5) grid of booleans, written
in `fillHashMap()` as a literal that visually matches how it prints:

```cpp
AsciiLetters['A'] = {{
    {0,0,1,0,0},
    {0,1,1,1,0},
    {0,1,0,1,0},
    {1,1,1,1,1},
    {1,0,0,0,1}
}};
```

`1` means "draw `fontChar`", `0` means "draw a space".

### Currently supported characters

- Letters: `A`–`Z` (uppercase only; lowercase input is uppercased before lookup)
- Symbols: `! = ? % & $ # + * ~`

Digits (`0`–`9`) and punctuation beyond the list above are **not** in the
table yet — see [Project Status](Project-Status.md).

## Adding a new character

1. Open `ctui/font/font.cpp` and add a new entry to `fillHashMap()`
   following the existing pattern — a 5×5 grid of 0/1 literals shaped like
   the glyph.
2. No changes are needed in `generateFont` or the bridge; both are
   character-agnostic.
3. Add a case to `ctui/tests/font_test.cpp` if the character should be
   covered by `FontTest.RendersKnownGlyph`-style assertions (see
   [Testing and CI](Testing-and-CI.md)).

## Tunable constants

Defined in `font.hpp`:

| Constant     | Meaning                                   | Default |
|--------------|--------------------------------------------|---------|
| `LINECOUNT`  | Glyph height (rows)                        | 5       |
| `FONTSIZE`   | Glyph width (columns)                      | 5       |
| `SPACECOUNT` | Spaces inserted between adjacent letters   | 1       |

Changing `LINECOUNT`/`FONTSIZE` requires re-authoring every glyph in
`fillHashMap()` to match the new grid dimensions.
