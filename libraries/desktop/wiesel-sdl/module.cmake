# wiesel-sdl library


# create the SDL module
wiesel_create_module(wiesel-sdl ${WIESEL_SRC_DIR}/sdl ${WIESEL_TESTS_DIR}/sdl)

# export include directories of this library for other targets
wiesel_module_export_includes(wiesel-sdl ${WIESEL_SRC_DIR}/sdl)

# add required modules
wiesel_module_add_dependency(wiesel-sdl wiesel-base)
wiesel_module_add_dependency(wiesel-sdl wiesel-core)
wiesel_module_add_dependency(wiesel-sdl wiesel-opengl)


# add SDL dependency
include(FindSDL)

if(SDL_FOUND)
	wiesel_target_add_includes(wiesel-sdl ${SDL_INCLUDE_DIR})
	wiesel_target_add_libraries(wiesel-sdl ${SDL_LIBRARY})
else(SDL_FOUND)
	message(FATAL_ERROR "required library SDL not found!")
endif(SDL_FOUND)


# add SDL_image dependency
include(FindSDL_image)

if(SDLIMAGE_FOUND)
	wiesel_target_add_libraries(wiesel-sdl ${SDLIMAGE_LIBRARY})
else(SDLIMAGE_FOUND)
	message(FATAL_ERROR "required library SDL_image not found!")
endif(SDLIMAGE_FOUND)
