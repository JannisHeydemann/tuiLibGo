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

/* TODO: your real C API, e.g.
 * typedef void *CtuiMenuHandle;
 * CtuiMenuHandle ctui_menu_new(void);
 * void           ctui_menu_add_item(CtuiMenuHandle h, const char *label);
 * void           ctui_menu_free(CtuiMenuHandle h);
 */

#ifdef __cplusplus
}
#endif

#endif /* CTUI_BRIDGE_H */
