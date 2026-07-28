// bridge.cpp — implements the C API from bridge.h as a shim over C++.
// Rules that bite:
//   * Anything you `new` here must be freed via a C free function you expose
//     (Go's GC can't see the C++ heap).
//   * Never let a C++ exception escape into Go — catch it here and convert
//     to an error code / out-param.
#include "bridge.h"
#include "font/ASCII-Art.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>
// #include "mainMenu/mainMenu.hpp"

#define CTUI_VERSION "0.1.0";

extern "C" {

// Returns the static version string; no allocation, nothing to free.
const char *ctui_version(void) {
    return CTUI_VERSION;
}

// Calls into generateFont() (font.hpp) to get one std::string-per-column
// per line, joins each line's columns into a single row string, then
// copies each row onto the C heap (malloc + strdup) since Go/cgo cannot
// hold a pointer into C++-managed memory. Paired with ctui_font_free().
CtuiFontLines ctui_font_render(const char *text, char fontChar) {
    std::vector<std::vector<std::string>> outVec;
    generateFont(text, strlen(text), fontChar, outVec);

    CtuiFontLines result;
    result.lineCount = (int)outVec.size();
    result.lines = (char **)malloc(sizeof(char*) * result.lineCount);
    for (int i = 0; i < result.lineCount; i++) {
        std::string row;
        for (auto &s : outVec[i]) row += s;
        result.lines[i] = static_cast<char *>(std::malloc(row.size() + 1));
        if (result.lines[i] != nullptr) {
            std::memcpy(result.lines[i], row.c_str(), row.size() + 1);
        }
    }
    return result;
}

// Frees every row string plus the row array itself. Must be called exactly
// once per CtuiFontLines produced by ctui_font_render().
void ctui_font_free(CtuiFontLines lines) {
    for (int i = 0; i < lines.lineCount; i++) free(lines.lines[i]);
    free(lines.lines);
}


}
