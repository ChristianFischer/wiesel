# wiesel-common library


# collect all files which belongs to this library
wiesel_create_module(wiesel-common ${WIESEL_SRC_DIR}/common ${WIESEL_TESTS_DIR}/common)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-common ${WIESEL_SRC_DIR}/common)

# add required modules
wiesel_module_add_dependency(wiesel-common wiesel-base)
wiesel_module_add_dependency(wiesel-common glee third-party/glee)



# add libxml2 dependency
include(FindLibXml2)

if(DEFINED LIBXML2_FOUND)
	wiesel_target_add_includes(wiesel-common ${LIBXML2_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-common ${LIBXML2_LIBRARIES})
	wiesel_target_add_compileflags(wiesel-common ${LIBXML2_DEFINITIONS})
else()
	message(FATAL_ERROR "required library libxml2 not found!")
endif()


# add SDL dependency
include(FindSDL)

if(DEFINED SDL_FOUND)
	wiesel_target_add_includes(wiesel-common ${SDL_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-common ${SDL_LIBRARY})
	wiesel_target_add_compileflags(wiesel-common "-DWIESEL_USE_LIBSDL")
else()
	message(FATAL_ERROR "required library SDL not found!")
endif()


# add SDL_image dependency
include(FindSDL_image)

if(DEFINED SDL_FOUND)
	wiesel_target_add_libraries(wiesel-common ${SDLIMAGE_LIBRARY})
else()
	message(FATAL_ERROR "required library SDL not found!")
endif()
