# wiesel-core library


# collect all files which belongs to this library
wiesel_create_module(wiesel-core ${WIESEL_SRC_DIR}/core ${WIESEL_TESTS_DIR}/core)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-core ${WIESEL_SRC_DIR}/core)

# add required modules
wiesel_module_add_dependency(wiesel-core wiesel-base)
