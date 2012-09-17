# wiesel-base library


# create the library
wiesel_create_module(wiesel-base ${WIESEL_SRC_DIR}/base ${WIESEL_TESTS_DIR}/base)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-base ${WIESEL_SRC_DIR}/base)
