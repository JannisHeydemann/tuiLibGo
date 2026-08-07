#include <gtest/gtest.h>
#include "../src/font/ASCII-Art.hpp"
#include <string>
#include <vector>
#include <array>

// Build the expected FONTSIZE-wide string for one row of one glyph.
static std::string renderRow(const std::array<bool, FONTSIZE>& row, char fontChar) {
    std::string s;
    for (bool cell : row) s += (cell ? fontChar : ' ');
    return s;
}

TEST(FontTest, Dimensions) {
    std::string text = "TEXT";
    std::vector<std::vector<std::string>> out;
    generateFont(text, static_cast<int>(text.size()), 'H', out);

    ASSERT_EQ(out.size(), static_cast<std::size_t>(LINECOUNT));
    for (const auto& line : out) {
        ASSERT_EQ(line.size(), text.size());                              // one slot per letter
        for (std::size_t col = 0; col < line.size(); col++) {
            // FONTSIZE wide, plus SPACECOUNT trailing spaces between letters (not after the last)
            std::size_t expected = FONTSIZE + (col + 1 < line.size() ? SPACECOUNT : 0);
            ASSERT_EQ(line[col].size(), expected);
        }
    }
}

TEST(FontTest, RendersKnownGlyph) {
    char fc = 'H';
    std::vector<std::vector<std::string>> out;
    generateFont("A", 1, fc, out);          // populates map as a side effect

    // Compare each rendered line of column 0 against glyph 'A'.
    for (int line = 0; line < LINECOUNT; line++) {
        std::string expected = renderRow(AsciiLetters['A'][line], fc);
        EXPECT_EQ(out[line][0], expected) << "mismatch on line " << line;
    }
}

TEST(FontTest, LowercaseIsUppercased) {
    std::vector<std::vector<std::string>> lo, hi;
    generateFont("a", 1, 'H', lo);
    generateFont("A", 1, 'H', hi);
    ASSERT_EQ(lo, hi);          // toupper should make these identical
}

TEST(FontTest, UsesFontCharNotHardcoded) {
    std::vector<std::vector<std::string>> out;
    generateFont("A", 1, '@', out);

    // Somewhere in the glyph there must be a '@', and never a '#'.
    bool foundFontChar = false;
    for (const auto& line : out) {
        for (const auto& cell : line) {
            if (cell.find('@') != std::string::npos) foundFontChar = true;
            EXPECT_EQ(cell.find('#'), std::string::npos) << "hardcoded '#' leaked in";
        }
    }
    EXPECT_TRUE(foundFontChar) << "fontChar '@' never appeared";
}

TEST(FontTest, MultipleLetters) {
    std::string text = "HI";
    std::vector<std::vector<std::string>> out;
    generateFont(text, static_cast<int>(text.size()), 'H', out);

    ASSERT_EQ(out.size(), static_cast<std::size_t>(LINECOUNT));
    for (const auto& line : out)
        EXPECT_EQ(line.size(), text.size());   // two columns, one per letter
}

TEST(FontTest, EmptyTextProducesNoColumns) {
    std::vector<std::vector<std::string>> out;
    generateFont("", 0, 'H', out);

    ASSERT_EQ(out.size(), static_cast<std::size_t>(LINECOUNT));
    for (const auto& line : out)
        EXPECT_TRUE(line.empty());
}

TEST(FontTest, UnsupportedCharacterRendersBlank) {
    // '.' has no entry in AsciiLetters; operator[] default-constructs an
    // all-false row for it, so it should render as FONTSIZE blank columns
    // rather than throwing or falling back to some other glyph.
    char fc = 'H';
    std::vector<std::vector<std::string>> out;
    generateFont(".", 1, fc, out);

    for (int line = 0; line < LINECOUNT; line++) {
        EXPECT_EQ(out[line][0], std::string(FONTSIZE, ' ')) << "mismatch on line " << line;
    }
}
