# wiesel-win32 library


# create the Win32 module
wiesel_create_module(wiesel-win32 ${WIESEL_SRC_DIR}/win32 ${WIESEL_TESTS_DIR}/win32)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-win32 ${WIESEL_SRC_DIR}/win32)

# add required modules
wiesel_module_add_dependency(wiesel-win32 wiesel-base)
wiesel_module_add_dependency(wiesel-win32 wiesel-core)

# add windows libraries
wiesel_target_add_libraries(wiesel-win32 gdiplus)
