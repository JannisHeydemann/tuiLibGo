/* bridge.h — C-compatible API exposed to Go via cgo.
 *
 * cgo speaks C, not C++. Everything Go calls must be declared here inside
 * extern "C" and implemented in bridge.cpp as a thin shim over your real
 * C++ classes. Use opaque `void*` handles for objects; never expose C++
 * types (std::string, classes, templates) across this boundary.
 */
#ifndef CTUI_BRIDGE_H
#define CTUI_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Example call so the whole pipeline compiles end to end.
 * Delete once you have real functions. */
const char *ctui_version(void);

/* One row of rendered, ready-to-print glyph text per element of `lines`.
 * Every pointer in `lines`, and `lines` itself, is heap-allocated on the
 * C++ side (malloc/strdup) — always pass the returned value to
 * ctui_font_free() exactly once to release it. */
typedef struct {
    char **lines;      // array of NUL-terminated C strings, one per row
    int lineCount;
} CtuiFontLines;

/* Renders `text` as a block/FIGlet-style banner using `fontChar` for the
 * "on" pixels (see generateFont() in font.hpp for the rendering rules).
 * Ownership of the returned CtuiFontLines transfers to the caller — free
 * it with ctui_font_free() when done. */
CtuiFontLines ctui_font_render(const char *text, char fontChar);

/* Releases a CtuiFontLines previously returned by ctui_font_render(). */
void ctui_font_free(CtuiFontLines lines);

#ifdef __cplusplus
}
#endif

#endif /* CTUI_BRIDGE_H */
