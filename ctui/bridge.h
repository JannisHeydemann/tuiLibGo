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

typedef struct {
    char **lines;      // array of NUL-terminated C strings, one per row
    int lineCount;
} CtuiFontLines;

CtuiFontLines ctui_font_render(const char *text, char fontChar);
void ctui_font_free(CtuiFontLines lines);

#ifdef __cplusplus
}
#endif

#endif /* CTUI_BRIDGE_H */
