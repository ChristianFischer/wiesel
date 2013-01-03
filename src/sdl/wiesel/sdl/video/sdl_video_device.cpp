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
#include "sdl_video_device.h"

#include <wiesel/util/log.h>
#include <wiesel/gl/gl.h>
#include <wiesel/ui/touchhandler.h>
#include <assert.h>

#include <SDL.h>


using namespace wiesel;
using namespace wiesel::sdl;
using namespace wiesel::video;



SdlVideoDevice::SdlVideoDevice(SdlPlatform *platform, Screen *screen) : VideoDevice(screen) {
	assert(platform);
	this->platform = platform;
	this->platform->addReceiver(this);
	return;
}


SdlVideoDevice::~SdlVideoDevice() {
	if (platform) {
		platform->removeReceiver(this);
	}

	return;
}


bool SdlVideoDevice::init(const dimension &size, unsigned int flags) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// set double buffer flag
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// create the opengl surface
	int w = size.width;
	int h = size.height;
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

	// video device is ready
	setState(Video_Active);

	// log OpenGL information
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Version:    %s", ((const char*)glGetString(GL_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Vendor:     %s", ((const char*)glGetString(GL_VENDOR)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Renderer:   %s", ((const char*)glGetString(GL_RENDERER)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Shader:     %s", ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Extensions: %s", ((const char*)glGetString(GL_EXTENSIONS)));
	CHECK_GL_ERROR;

	return true;
}


bool SdlVideoDevice::shutdown() {
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	return true;
}


void SdlVideoDevice::preRender() {
	// Just fill the screen with a color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CHECK_GL_ERROR;

	return;
}


void SdlVideoDevice::postRender() {
	SDL_GL_SwapBuffers();
	CHECK_GL_ERROR;
	return;
}


void SdlVideoDevice::onEvent(const SDL_Event& event) {
	switch(event.type) {
		case SDL_MOUSEBUTTONDOWN: {
			getScreen()->getTouchHandler()->startTouch(event.button.button, event.button.x, event.button.y);
			break;
		}

		case SDL_MOUSEMOTION: {
			for(int button=0; button<5; button++) {
				getScreen()->getTouchHandler()->updateTouchLocation(button, event.motion.x, event.motion.y);
			}

			break;
		}

		case SDL_MOUSEBUTTONUP: {
			getScreen()->getTouchHandler()->releaseTouch(event.button.button);
			break;
		}
	}

	return;
}
