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
#include "../widgets/box.hpp"       // Brings in the Box widget (border + title)

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

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
    if (width <= 0 || height <= 0) {
        return nullptr; // Reject invalid dimensions before touching TuiEngineImpl/Canvas
    }
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
        // Canvas dropped its drawText() convenience method when text/box
        // rendering moved to the widgets/ hierarchy, so plain (non-widget)
        // text goes straight through setChar() the same way it used to.
        std::string s(text);
        for (size_t i = 0; i < s.length(); ++i) {
            impl->canvas.setChar(x + static_cast<int>(i), y, s[i]);
        }
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
    // Box::setWidth()/setHeight() silently no-op on sizes too small to hold
    // a border (see widgets.cpp), and Widget has no constructor to give
    // width/height a default -- so reject before ever touching the widget,
    // the same guard Canvas::drawBox() used to run first.
    if (width <= 2 || height < 3) return;
    std::string titleStr = text ? text : "";
    try {
        TuiEngineImpl *impl = static_cast<TuiEngineImpl*>(handle);
        Box box;
        box.setXCoords(x);
        box.setYCoords(y);
        box.setWidth(width);
        box.setHeight(height);
        box.setBorder(border);
        box.setTitle(titleStr); // silently left blank if too long to fit
        box.draw(impl->canvas);
    } catch (...) {}
}

} // extern "C"
