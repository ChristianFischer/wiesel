# wiesel-base library


# create the library
wiesel_create_module(wiesel-base ${WIESEL_SRC_DIR}/base *.cpp)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-base ${WIESEL_SRC_DIR}/base)
