# wiesel-common library


# create the Extras module
wiesel_create_module(wiesel-common ${WIESEL_SRC_DIR}/common ${WIESEL_TESTS_DIR}/common)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-common ${WIESEL_SRC_DIR}/common)

# add required modules
wiesel_module_add_dependency(wiesel-common wiesel-base)
wiesel_module_add_dependency(wiesel-common wiesel-core)


# add libxml2 dependency
include(FindLibXml2)

if (LIBXML2_FOUND)
	wiesel_target_add_includes(wiesel-common ${LIBXML2_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-common ${LIBXML2_LIBRARIES})
	wiesel_target_add_compileflags(wiesel-common ${LIBXML2_DEFINITIONS})

	set(WIESEL_SUPPORTS_LIBXML2		TRUE)
else(LIBXML2_FOUND)
	set(WIESEL_SUPPORTS_LIBXML2		FALSE)
endif(LIBXML2_FOUND)


# check if libpng is available
include(FindPNG)

if(PNG_FOUND)
	wiesel_target_add_includes(wiesel-common ${PNG_INCLUDE_DIRS})
	wiesel_target_add_libraries(wiesel-common ${PNG_LIBRARIES})
	wiesel_target_add_compileflags(wiesel-common ${PNG_DEFINITIONS})

	set(WIESEL_SUPPORTS_LIBPNG		TRUE)
else()
	set(WIESEL_SUPPORTS_LIBPNG		FALSE)
endif()


# finally, create the config file
configure_file(
		${WIESEL_SRC_DIR}/common/wiesel/wiesel-common-config.in
		${WIESEL_GENERATED_DIR}/wiesel/wiesel-common-config.h
)
