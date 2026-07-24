#include "font.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <array>

// Hashmap of the characters
std::unordered_map<char, std::array<std::array<bool, FONTSIZE>, LINECOUNT>> AsciiLetters;

// See font.hpp for the full parameter contract. Implementation walks the
// output line-by-line (outer loop) and, within each line, letter-by-letter
// (inner loop), converting that letter's boolean row from AsciiLetters into
// a string of fontChar/space cells and appending the inter-letter spacing.
// A char missing from AsciiLetters looks up as a default-constructed
// (all-false) row via operator[], i.e. it silently renders as blank columns.
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

// Assigns a LINECOUNT x FONTSIZE boolean grid to every supported character:
// A-Z plus a handful of symbols (! = ? % & $ # + * ~). Each grid literal
// below is laid out visually top-to-bottom, matching how it prints, with
// 1 meaning "draw fontChar" and 0 meaning "draw a space". Idempotent —
// safe to call more than once, though generateFont() only does so on its
// first call (see the `initialized` guard above).
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

    AsciiLetters['1'] = {{
        {0,0,1,0,0},
        {0,1,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,1,1,1,0}
    }};

    AsciiLetters['2'] = {{
        {0,0,1,1,0},
        {0,1,0,0,1},
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,1,1,1,1}
    }};

    AsciiLetters['3'] = {{
        {0,1,1,1,0},
        {0,0,0,0,1},
        {0,1,1,1,0},
        {0,0,0,0,1},
        {0,1,1,1,0}
    }};

    AsciiLetters['4'] = {{
        {0,0,0,1,0},
        {0,0,1,1,0},
        {0,1,0,1,0},
        {1,1,1,1,1},
        {0,0,0,1,0}
    }};

    AsciiLetters['5'] = {{
        {1,1,1,1,1},
        {1,0,0,0,0},
        {1,1,1,1,0},
        {0,0,0,0,1},
        {1,1,1,1,0}
    }};

    AsciiLetters['6'] = {{
        {0,1,1,1,1},
        {1,0,0,0,0},
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0}
    }};

    AsciiLetters['7'] = {{
        {1,1,1,1,1},
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,1,0,0,0},
        {1,0,0,0,0}
    }};

    AsciiLetters['8'] = {{
        {0,1,1,1,0},
        {1,0,0,0,1},
        {0,1,1,1,0},
        {1,0,0,0,1},
        {0,1,1,1,0}
    }};

    AsciiLetters['9'] = {{
        {0,1,1,1,1},
        {1,0,0,0,1},
        {0,1,1,1,1},
        {0,0,0,0,1},
        {1,1,1,1,0}
    }};

    AsciiLetters['0'] = {{
        {0,1,1,1,0},
        {1,0,0,1,1},
        {1,0,1,0,1},
        {1,1,0,0,1},
        {0,1,1,1,0}
    }};
}
