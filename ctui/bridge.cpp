// bridge.cpp — implements the C API from bridge.h as a shim over C++.
// Rules that bite:
//   * Anything you `new` here must be freed via a C free function you expose
//     (Go's GC can't see the C++ heap).
//   * Never let a C++ exception escape into Go — catch it here and convert
//     to an error code / out-param.
#include "bridge.h"
#include "font/font.hpp"
#include <cstdio>
#include <cstring>
// #include "mainMenu/mainMenu.hpp"

#define CTUI_VERSION "0.1.0";

extern "C" {

const char *ctui_version(void) {
    return CTUI_VERSION;
}

CtuiFontLines ctui_font_render(const char *text, char fontChar) {
    std::vector<std::vector<std::string>> outVec;
    generateFont(text, strlen(text), fontChar, outVec);

    CtuiFontLines result;
    result.lineCount = (int)outVec.size();
    result.lines = (char **)malloc(sizeof(char*) * result.lineCount);
    for (int i = 0; i < result.lineCount; i++) {
        std::string row;
        for (auto &s : outVec[i]) row += s;
        result.lines[i] = strdup(row.c_str());
    }
    return result;
}

void ctui_font_free(CtuiFontLines lines) {
    for (int i = 0; i < lines.lineCount; i++) free(lines.lines[i]);
    free(lines.lines);
}


}
