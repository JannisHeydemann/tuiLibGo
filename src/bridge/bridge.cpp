// bridge.cpp — implements the C API from bridge.h as a shim over C++.
// Rules that bite:
//   * Anything you `new` here must be freed via a C free function you expose
//     (Go's GC can't see the C++ heap).
//   * Never let a C++ exception escape into Go — catch it here and convert
//     to an error code / out-param.
#include "bridge.h"
#include "../font/ASCII-Art.hpp"
#include "../terminal/terminal.hpp" // Brings in your Terminal (raw mode setup)
#include "../canvas/canvas.hpp"     // Brings in your Canvas (2D buffer grid)

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <exception>

#define CTUI_VERSION "0.1.0"

// Internal C++ structure holding both the raw mode lifecycle (Terminal)
// and the double-buffering canvas engine together.
struct TuiEngineImpl {
    Terminal terminal;
    Canvas canvas;

    TuiEngineImpl(int w, int h) : canvas(w, h) {}
};

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

// =========================================================================
// TUI Engine Implementation
// =========================================================================

CTuiEngine ctui_engine_create(int width, int height) {
    try {
        // Instantiating TuiEngineImpl triggers Terminal's constructor,
        // putting the terminal in raw mode & entering the alternate buffer automatically.
        TuiEngineImpl *impl = new TuiEngineImpl(width, height);
        return static_cast<CTuiEngine>(impl);
    } catch (...) {
        return nullptr; // Return null to Go on allocation/setup failure
    }
}

void ctui_engine_destroy(CTuiEngine handle) {
    if (!handle) return;

    TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
    // Deleting impl fires ~Terminal(), which restores raw mode back to canonical,
    // reveals the cursor, and exits the alternate screen buffer cleanly!
    delete impl;
}

void ctui_engine_clear(CTuiEngine handle) {
    if (!handle) return;
    try {
        TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
        impl->canvas.clear();
    } catch (...) {}
}

void ctui_engine_draw_text(CTuiEngine handle, int x, int y, const char *text) {
    if (!handle || !text) return;
    try {
        TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
        impl->canvas.drawText(x, y, std::string(text));
    } catch (...) {}
}

void ctui_engine_render(CTuiEngine handle) {
    if (!handle) return;
    try {
        TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
        impl->canvas.render();
    } catch (...) {}
}

void ctui_engine_draw_box(CTuiEngine handle, int x, int y, int width, int height, const char *text, const char border) {
    if (!handle) return;
    std::string titleStr = text ? text : "";
    try {
        TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
        impl->canvas.drawBox(x, y, width, height, std::string(text), border);
    } catch (...) {}
}

} // extern "C"
