#include <gtest/gtest.h>
#include "../font/font.hpp"
#include <string>
#include <vector>

TEST(FontTest, Constructs) {
    std::string testText = "text";

    int testTextLength = testText.size();
    char fontChar = '#';

    int width = testTextLength * FONTSIZE + (testTextLength - 1);  // 19 for "text"
    std::vector<std::vector<std::string>> expectedVec(LINECOUNT, std::vector<std::string>(width, "#"));
    std::vector<std::vector<std::string>> outputVec(LINECOUNT, std::vector<std::string>(width));

    for (int i = 0; i < LINECOUNT; i++) {
        for (int j = 0; j < 19; j++) {
            expectedVec[i][j] = "#";
        }
    }


    generateFont(testText, testTextLength, fontChar, outputVec);
    ASSERT_EQ(outputVec, expectedVec);
}
