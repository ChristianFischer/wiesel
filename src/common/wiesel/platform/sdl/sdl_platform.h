/**
 * Copyright (C) 2012
 * Christian Fischer
 *
 * https://bitbucket.org/baldur/wiesel/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#ifndef __WIESEL_PLATFORM_SDL_PLATFORM_MAIN_H__
#define __WIESEL_PLATFORM_SDL_PLATFORM_MAIN_H__

#include "../../application.h"

#if WIESEL_USE_LIBSDL

/// application entry point macro
#define	WIESEL_APPLICATION_SETUP(APPLICATION)						\
	extern "C" int main(int argc, char* argv[]) {					\
		APPLICATION app = APPLICATION();							\
		__internal_sdl_app_main(&app, argc, argv);					\
		return 0;													\
	}

/// delegate for the application entry point
void __internal_sdl_app_main(wiesel::Application *app, int argc, char* argv[]);

#endif // WIESEL_USE_LIBSDL

#endif // __WIESEL_PLATFORM_SDL_PLATFORM_MAIN_H__