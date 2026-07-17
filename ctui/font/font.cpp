#include "font.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <array>

// Hashmap of the characters
std::unordered_map<char, std::array<std::array<bool, FONTSIZE>, LINECOUNT>> AsciiLetters;

void generateFont(std::string text, int textLength, char fontChar, std::vector<std::vector<std::string>>& outVec) {

    static bool initialized = false;
    if (!initialized) {
        fillHashMap();
        initialized = true;
    }

    outVec.assign(LINECOUNT, std::vector<std::string>(textLength, ""));
    
    for (int LineCounter = 0; LineCounter < LINECOUNT; LineCounter++) {
        for (int LetterCounter = 0; LetterCounter < textLength; LetterCounter++) {
            char CurrChar = static_cast<char>(std::toupper(static_cast<unsigned char>(text[LetterCounter])));
            for (bool cell: AsciiLetters[CurrChar][LineCounter]) {
                outVec[LineCounter][LetterCounter] += (cell ? fontChar : ' ');
            }
            if (LetterCounter < textLength - 1) {
                outVec[LineCounter][LetterCounter] += std::string(SPACECOUNT, ' ');
            }
        }
    }
}

// current thought is: having a 2d array filled with booleans for every letter.
// Then we go line by line and add every line, add the font line to the 2d vector and add 2 spaces and then continue with the next letter.
// When the first line is written, we go to the next and repeat everything
// put all letters in hashmap
void fillHashMap() {
    AsciiLetters['A'] = {{
        {0,0,1,0,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {1,1,1,1,1},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['B'] = {{
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,1,1,1,0}
    }};
    
    AsciiLetters['C'] = {{
        {0,1,1,1,1},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {0,1,1,1,0}
    }};
    
    AsciiLetters['D'] = {{
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,1,1,1,0}
    }};
    
    AsciiLetters['E'] = {{
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['F'] = {{
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,0,0,0,0}
    }};
    
    AsciiLetters['G'] = {{
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,0,0,1,1},
        {1,0,0,0,1},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['H'] = {{
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['I'] = {{
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['J'] = {{
        {1,1,1,1,1},
        {0,0,0,0,1},
        {0,0,0,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['K'] = {{
        {1,0,0,1,0},
        {1,0,1,0,0},
        {1,1,0,0,0},
        {1,0,1,1,0},
        {1,0,0,1,1}
    }};
    
    AsciiLetters['L'] = {{
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['M'] = {{
        {1,1,0,1,1},
        {1,0,1,0,1},
        {1,0,1,0,1},
        {1,0,0,0,1},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['N'] = {{
        {1,0,0,0,1},
        {1,1,0,0,1},
        {1,0,1,0,1},
        {1,0,0,1,1},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['O'] = {{
        {0,1,1,1,0},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {0,1,1,1,0}
    }};
    
    AsciiLetters['P'] = {{
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0},
        {1,0,0,0,0},
        {1,0,0,0,0}
    }};
    
    AsciiLetters['Q'] = {{
        {0,1,1,1,0},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,0,1,0},
        {0,1,1,0,1}
    }};
    
    AsciiLetters['R'] = {{
        {0,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0},
        {1,0,0,1,0},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['S'] = {{
        {0,1,1,1,1},
        {1,0,0,0,0},
        {0,1,1,1,0},
        {0,0,0,0,1},
        {1,1,1,1,0}
    }};
    
    AsciiLetters['T'] = {{
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    }};
    
    AsciiLetters['U'] = {{
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,0,0,1},
        {0,1,1,1,0}
    }};
    
    AsciiLetters['V'] = {{
        {1,0,0,0,1},
        {1,0,0,0,1},
        {0,1,0,1,0},
        {0,1,0,1,0},
        {0,0,1,0,0}
    }};
    
    AsciiLetters['W'] = {{
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,0,1,0,1},
        {1,0,1,0,1},
        {0,1,0,1,0}
    }};
    
    AsciiLetters['X'] = {{
        {1,0,0,0,1},
        {0,1,0,1,0},
        {0,0,1,0,0},
        {0,1,0,1,0},
        {1,0,0,0,1}
    }};
    
    AsciiLetters['Y'] = {{
        {1,0,0,0,1},
        {0,1,0,1,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    }};
    
    AsciiLetters['Z'] = {{
        {1,1,1,1,1},
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,1,0,0,0},
        {1,1,1,1,1}
    }};
    
    AsciiLetters['!'] = {{
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    }};

    AsciiLetters['='] = {{
        {0,0,0,0,0},
        {1,1,1,1,1},
        {0,0,0,0,0},
        {1,1,1,1,1},
        {0,0,0,0,0}
    }};

    AsciiLetters['?'] = {{
        {1,1,1,1,0},
        {0,0,0,0,1},
        {0,0,1,1,0},
        {0,0,0,0,0},
        {0,0,1,0,0}
    }};

    AsciiLetters['%'] = {{
        {1,0,0,0,1},
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,1,0,0,0},
        {1,0,0,0,1}
    }};

    AsciiLetters['&'] = {{
        {0,1,1,0,0},
        {1,0,0,1,0},
        {0,1,1,0,0},
        {1,0,0,1,0},
        {0,1,1,0,1}
    }};

    AsciiLetters['$'] = {{
        {0,1,1,1,1},
        {1,0,1,0,0},
        {0,1,1,1,0},
        {0,0,1,0,1},
        {1,1,1,1,0}
    }};

    AsciiLetters['#'] = {{
        {0,1,0,1,0},
        {1,1,1,1,1},
        {0,1,0,1,0},
        {1,1,1,1,1},
        {0,1,0,1,0}
    }};

    AsciiLetters['+'] = {{
        {0,0,1,0,0},
        {0,0,1,0,0},
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0}
    }};

    AsciiLetters['*'] = {{
        {0,0,1,0,0},
        {1,0,1,0,1},
        {0,1,1,1,0},
        {1,0,1,0,1},
        {0,0,1,0,0}
    }};

    AsciiLetters['~'] = {{
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,1,1,0,0},
        {1,0,0,1,1},
        {0,0,0,0,0}
    }};
    
}
