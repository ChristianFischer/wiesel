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
#include "sdl_video_driver.h"

#include <wiesel/video/gl/gl.h>
#include <wiesel/util/log.h>
#include <wiesel/ui/touchhandler.h>
#include <assert.h>

#include <SDL.h>


using namespace wiesel;
using namespace wiesel::sdl;
using namespace wiesel::video;
using namespace wiesel::video::gl;



SdlVideoDeviceDriver::SdlVideoDeviceDriver(SdlPlatform *platform, wiesel::video::Screen *screen) : OpenGlVideoDeviceDriver(screen) {
	assert(platform);
	this->platform = platform;
	this->platform->addReceiver(this);

	this->render_context = NULL;

	return;
}


SdlVideoDeviceDriver::~SdlVideoDeviceDriver() {
	if (platform) {
		platform->removeReceiver(this);
	}
	
	safe_release(render_context);

	return;
}


bool SdlVideoDeviceDriver::init(const dimension &size, unsigned int flags) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logmsg(LogLevel_Error, WIESEL_SDL_LOG_TAG, "Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// set double buffer flag
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// create the opengl surface
	int w = size.width;
	int h = size.height;
	SDL_Surface* screen = SDL_SetVideoMode(w, h, 16, SDL_OPENGL);
	if (!screen) {
		logmsg(LogLevel_Error, WIESEL_SDL_LOG_TAG, "Unable to set video mode: %s\n", SDL_GetError());
		return false;
	}

	// try to initialize the OpenGL part
	if (!initOpenGl()) {
		return false;
	}

	// remove the old context, if any
	safe_release(render_context);

	// initialize the new render context
	render_context = keep(new SdlGlRenderContext(getScreen()));
	render_context->initContext();

	// update screen size and projection
	updateScreenSize(w, h);

	// video device is ready
	setState(Video_Active);

	return true;
}


bool SdlVideoDeviceDriver::shutdown() {
	safe_release(render_context);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);

	return true;
}


void SdlVideoDeviceDriver::preRender() {
	OpenGlVideoDeviceDriver::preRender();

	if (render_context) {
		render_context->preRender();
		render_context->setProjectionMatrix(projection);
	}

	return;
}


void SdlVideoDeviceDriver::postRender() {
	if (render_context) {
		render_context->postRender();
	}

	SDL_GL_SwapBuffers();
	CHECK_GL_ERROR;

	OpenGlVideoDeviceDriver::postRender();

	return;
}


RenderContext *SdlVideoDeviceDriver::getCurrentRenderContext() {
	return render_context;
}



void SdlVideoDeviceDriver::onEvent(const SDL_Event& event) {
	switch(event.type) {
		case SDL_MOUSEBUTTONDOWN: {
			vector2d location = convertScreenToWorld(vector2d(event.button.x, event.button.y));
			getScreen()->getTouchHandler()->startTouch(event.button.button - 1, location.x, location.y);
			break;
		}

		case SDL_MOUSEMOTION: {
			vector2d location = convertScreenToWorld(vector2d(event.motion.x, event.motion.y));

			for(int button=1; button<=5; button++) {
				getScreen()->getTouchHandler()->updateTouchLocation(button - 1, location.x, location.y);
			}

			break;
		}

		case SDL_MOUSEBUTTONUP: {
			getScreen()->getTouchHandler()->releaseTouch(event.button.button - 1);
			break;
		}
	}

	return;
}
