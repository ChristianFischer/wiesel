# wiesel-core library


# collect all files which belongs to this library
wiesel_create_module(wiesel-core ${WIESEL_SRC_DIR}/core ${WIESEL_TESTS_DIR}/core)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-core ${WIESEL_SRC_DIR}/core)

# add required modules
wiesel_module_add_dependency(wiesel-core wiesel-base)


# add libxml2 dependency
include(FindLibXml2)

if(DEFINED LIBXML2_FOUND)
	wiesel_target_add_includes(wiesel-core ${LIBXML2_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-core ${LIBXML2_LIBRARIES})
	wiesel_target_add_compileflags(wiesel-core ${LIBXML2_DEFINITIONS})
else()
	message(FATAL_ERROR "required library libxml2 not found!")
endif()
