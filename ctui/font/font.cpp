#include "font.hpp"
#include <vector>
#include <string>

void generateFont([[maybe_unused]] std::string text, int textLength, char fontChar, std::vector<std::vector<std::string>>& outVec) {
    // length of text * fontSize as FONTSIZE describes the width of one char and add (textLength - 1) * SPACECOUNT because their will be (textLength - 1) * SPACECOUNT spaces in the end
    int charAmount = textLength * FONTSIZE + ((textLength - 1) * SPACECOUNT);
    for (int i = 0; i < LINECOUNT; i++) {
        for (int j = 0; j < charAmount; j++) {
            outVec[i][j] = fontChar;

        }
    }
}

// current thought is: having a 2d array for every letter of booleans. then we go line by line and add every line, add the font line to the 2d vector and add 2 spaces and then off to the next
