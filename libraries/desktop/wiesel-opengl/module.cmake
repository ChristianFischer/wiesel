# wiesel-opengl library


# create the OpenGL module
wiesel_create_module(wiesel-opengl ${WIESEL_SRC_DIR}/opengl ${WIESEL_TESTS_DIR}/opengl)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-opengl ${WIESEL_SRC_DIR}/opengl)

# add required modules
wiesel_module_add_dependency(wiesel-opengl wiesel-base)
wiesel_module_add_dependency(wiesel-opengl wiesel-core)
wiesel_module_add_dependency(wiesel-opengl glee third-party/glee)
