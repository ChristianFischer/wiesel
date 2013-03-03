set(WIESEL_ADD_MODULE_SDL_DEFAULT		ON)
set(WIESEL_ADD_MODULE_SDL2_DEFAULT		OFF)
set(WIESEL_ADD_MODULE_WIN32_DEFAULT		OFF)


if (WIN32)
	set(WIESEL_ADD_MODULE_WIN32_DEFAULT		ON)
endif(WIN32)



option(
		WIESEL_ADD_MODULE_SDL
		"Add the SDL module to the project. The SDL provides an OpenGL based renderer and an image loader for some common image formats. This will also include the OpenGL module"
		${WIESEL_ADD_MODULE_SDL_DEFAULT}
)

option(
		WIESEL_ADD_MODULE_SDL2
		"Add the SDL module for libSDL version 2 to the project. The SDL provides an OpenGL based renderer, so this will also include the OpenGL module. It does not provide image loading modules like the SDL 1.x module."
		${WIESEL_ADD_MODULE_SDL2_DEFAULT}
)

option(
		WIESEL_ADD_MODULE_WIN32
		"This module provides some windows specific extensions, for example the ability for loading images using GDI+."
		${WIESEL_ADD_MODULE_WIN32_DEFAULT}
)



function(wiesel_default_project_setup target)
	set(MAINPROC_REQUIRED FALSE)

	# common modules, which are included always
	wiesel_module_add_dependency(${target} wiesel-base)
	wiesel_module_add_dependency(${target} wiesel-core)
	wiesel_module_add_dependency(${target} wiesel-common)
	
	if (WIESEL_ADD_MODULE_SDL)
		wiesel_module_add_dependency(${target} wiesel-sdl)
		set(MAINPROC_REQUIRED TRUE)
	endif(WIESEL_ADD_MODULE_SDL)
	
	if (WIESEL_ADD_MODULE_SDL2)
		wiesel_module_add_dependency(${target} wiesel-sdl2)
		set(MAINPROC_REQUIRED TRUE)
	endif(WIESEL_ADD_MODULE_SDL2)
	
	if (WIESEL_ADD_MODULE_WIN32)
		wiesel_module_add_dependency(${target} wiesel-win32)
	endif(WIESEL_ADD_MODULE_WIN32)
	
	if (MAINPROC_REQUIRED)
		wiesel_module_add_dependency(${target} wiesel-mainproc)
	endif(MAINPROC_REQUIRED)
	
endfunction(wiesel_default_project_setup)
