# wiesel-sdl2 library


# create the SDL2 module
wiesel_create_module(wiesel-sdl2 ${WIESEL_SRC_DIR}/sdl2 ${WIESEL_TESTS_DIR}/sdl2)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-sdl2 ${WIESEL_SRC_DIR}/sdl2)

# add required modules
wiesel_module_add_dependency(wiesel-sdl2 wiesel-base)
wiesel_module_add_dependency(wiesel-sdl2 wiesel-core)
wiesel_module_add_dependency(wiesel-sdl2 wiesel-opengl)


# add SDL dependency
include(wiesel-FindSDL2)

if(SDL2_FOUND)
	if (DEFINED SDL2_DEPENDENCY)
		add_dependencies(wiesel-sdl2 ${SDL2_DEPENDENCY})
	endif(DEFINED SDL2_DEPENDENCY)

	wiesel_target_add_includes(wiesel-sdl2 ${SDL2_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-sdl2 ${SDL2_LIBRARY})
else(SDL2_FOUND)
	message(FATAL_ERROR "required library SDL2 not found!")
endif(SDL2_FOUND)
