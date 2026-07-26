#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <vector>

// Height of Font
int const LINECOUNT = 5;

// width of font
int const FONTSIZE = 5;

// amount of spaces between each letter
int const SPACECOUNT = 1;

// Glyph table: maps an uppercase character (or supported symbol) to a
// LINECOUNT x FONTSIZE grid of booleans, where `true` marks a pixel that
// should be drawn with the caller's chosen fontChar. Populated once by
// fillHashMap() on first use; see font.cpp for the full character set.
extern std::unordered_map<char, std::array<std::array<bool, FONTSIZE>, LINECOUNT>> AsciiLetters;

///////////////
// functions //
///////////////

// Renders `text` as a block/FIGlet-style banner.
//
// text:        the string to render; letters are matched case-insensitively
//              (internally uppercased) against AsciiLetters. Characters with
//              no glyph entry render as blank cells.
// textLength:  number of characters from `text` to render (also the number
//              of columns allocated in outVec).
// fontChar:    the character used to draw "on" pixels; "off" pixels are spaces.
// outVec:      output parameter, overwritten on each call. Sized to
//              LINECOUNT rows x textLength columns; outVec[line][letter] is
//              the fully-rendered FONTSIZE-wide (plus trailing SPACECOUNT
//              spaces, except after the last letter) string for that glyph row.
//
// Lazily calls fillHashMap() exactly once across all invocations.
void generateFont(std::string text, int textLength, char fontChar, std::vector<std::vector<std::string>>& outVec);

// Populates the AsciiLetters glyph table with every supported character.
// Safe to call multiple times (it just re-assigns the same entries); called
// automatically by generateFont() the first time it runs.
void fillHashMap();
