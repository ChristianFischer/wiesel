# wiesel-net library


# collect all files which belongs to this library
wiesel_create_module(wiesel-net ${WIESEL_SRC_DIR}/net ${WIESEL_TESTS_DIR}/net)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-net ${WIESEL_SRC_DIR}/net)

# add required modules
wiesel_module_add_dependency(wiesel-net wiesel-base)
wiesel_module_add_dependency(wiesel-net wiesel-core)

if (WIN32)
	# link winsock library
	target_link_libraries(wiesel-net ws2_32)
endif(WIN32)
