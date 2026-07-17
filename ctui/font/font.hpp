#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Height of Font
int const LINECOUNT = 5;

// width of font
int const FONTSIZE = 5;

// amount of spaces between each letter
int const SPACECOUNT = 1;

// hashmap of the letters
extern std::unordered_map<char, std::array<std::array<bool, FONTSIZE>, LINECOUNT>> AsciiLetters;

///////////////
// functions //
///////////////

// generates the PIGlet style drawing
void generateFont(std::string text, int size, char fontChar, std::vector<std::vector<std::string>>& outVec);

// fills the hashmap with according values
void fillHashMap();
