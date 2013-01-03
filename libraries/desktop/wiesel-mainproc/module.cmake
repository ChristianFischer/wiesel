# wiesel-mainproc library


# create the main module
wiesel_create_module(wiesel-mainproc ${WIESEL_SRC_DIR}/mainproc ${WIESEL_TESTS_DIR}/mainproc STATIC)

# add required modules
wiesel_module_add_dependency(wiesel-mainproc wiesel-base)
wiesel_module_add_dependency(wiesel-mainproc wiesel-core)

