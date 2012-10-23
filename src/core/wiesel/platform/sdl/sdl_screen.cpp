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
#if WIESEL_USE_LIBSDL

#include "sdl_screen.h"
#include <wiesel/util/log.h>
#include <wiesel/gl/gl.h>
#include <assert.h>

#include <SDL.h>


using namespace wiesel;



SdlScreen::SdlScreen()
: engine(NULL)
{
	return;
}


SdlScreen::SdlScreen(SdlEngine *engine)
: engine(engine)
{
	return;
}


SdlScreen::~SdlScreen() {
	return;
}


bool SdlScreen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// set double buffer flag
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// create the opengl surface
	int w = 640;
	int h = 480;
	SDL_Surface* screen = SDL_SetVideoMode(w, h, 16, SDL_OPENGL);
	if (!screen) {
		logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Unable to set video mode: %s\n", SDL_GetError());
		return false;
	}

	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup viewport
	glViewport(0, 0, w, h);

	// update screen size and projection
	updateScreenSize(w, h);

	// log OpenGL information
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Version:    %s", ((const char*)glGetString(GL_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Vendor:     %s", ((const char*)glGetString(GL_VENDOR)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Renderer:   %s", ((const char*)glGetString(GL_RENDERER)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Shader:     %s", ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Extensions: %s", ((const char*)glGetString(GL_EXTENSIONS)));
	CHECK_GL_ERROR;

	return true;
}


bool SdlScreen::release() {
	return true;
}


void SdlScreen::preRender() {
	// Just fill the screen with a color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CHECK_GL_ERROR;

	return;
}


void SdlScreen::postRender() {
	SDL_GL_SwapBuffers();
	CHECK_GL_ERROR;
	return;
}

#endif // WIESEL_USE_LIBSDL
