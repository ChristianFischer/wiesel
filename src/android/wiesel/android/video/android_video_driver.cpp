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
#include "android_video_driver.h"
#include <wiesel/video/gl/gl.h>
#include <wiesel/ui/touchhandler.h>
#include <wiesel/util/log.h>
#include <assert.h>


using namespace wiesel;
using namespace wiesel::android;
using namespace wiesel::video;



AndroidVideoDeviceDriver::AndroidVideoDeviceDriver(AndroidPlatform *platform, Screen *screen) : OpenGlVideoDeviceDriver(screen) {
	this->display		= EGL_NO_DISPLAY;
	this->surface		= EGL_NO_SURFACE;
	this->context		= EGL_NO_CONTEXT;
	this->config		= NULL;

	render_context		= NULL;

	this->platform		= platform;
	this->platform->addReceiver(this);

	return;
}


AndroidVideoDeviceDriver::~AndroidVideoDeviceDriver() {
	if (this->platform) {
		this->platform->removeReceiver(this);
	}

	// release the render context
	safe_release(render_context);

	// check if the screen was released correctly
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);
	assert(config  == NULL);

	return;
}


bool AndroidVideoDeviceDriver::init() {
	// check if the screen is not already initialized
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);
	assert(config  == NULL);

	if (
			platform
		&&	platform->getAndroidApp()
		&&	platform->getAndroidApp()->window
	) {
		if (!initContext()) {
			return false;
		}
	}
	else {
		setState(Video_Uninitialized);
	}

	return true;
}

bool AndroidVideoDeviceDriver::initContext() {
	// initialize OpenGL ES and EGL

	// get the android app object
	assert(platform);
	struct android_app *app = platform->getAndroidApp();
	assert(app);

	logmsg(LogLevel_Info, "TEST", "init context, window=%p", app->window);

	EGLDisplay default_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (this->display != EGL_NO_DISPLAY && this->display != default_display) {
		releaseContext();
	}

	if (display == EGL_NO_DISPLAY) {
		display =  default_display;
		eglInitialize(display, 0, 0);
	}

	if (config == NULL) {
		/*
		 * Here specify the attributes of the desired configuration.
		 * Below, we select an EGLConfig with at least 8 bits per color
		 * component compatible with on-screen windows
		 */
		const EGLint config_attribs[] = {
				EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
				EGL_BLUE_SIZE,			8,
				EGL_GREEN_SIZE,			8,
				EGL_RED_SIZE,			8,
				EGL_DEPTH_SIZE,			16,
				EGL_NONE
		};

		EGLint numConfigs;

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria
		 */
		eglChooseConfig(display, config_attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
		 */
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	}

	if (surface == EGL_NO_SURFACE) {
		ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);
		
		surface = eglCreateWindowSurface(display, config, app->window, NULL);
	}

	if (context == EGL_NO_CONTEXT) {
		const EGLint context_attribs[] = {
				EGL_CONTEXT_CLIENT_VERSION,		2,
				EGL_NONE
		};

		context = eglCreateContext(display, config, NULL, context_attribs);
		CHECK_GL_ERROR;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "eglMakeCurrent failed!");
		return false;
	}


	// try to initialize the OpenGL part
	if (!initOpenGl()) {
		return false;
	}

	// remove the old context, if any
	safe_release(render_context);

	// initialize the new render context
	render_context = keep(new AndroidGlRenderContext(getScreen()));
	render_context->initContext();

	// initialize the window's size
	resize();

	// video device is ready
	setState(Video_Active);

	CHECK_GL_ERROR;

	return true;
}


bool AndroidVideoDeviceDriver::resize() {
	EGLint w, h;

	// get the display size
	eglQuerySurface(display, surface, EGL_WIDTH,  &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	CHECK_GL_ERROR;

	// setup viewport
	glViewport(0, 0, w, h);

	// update screen size and projection
	updateScreenSize(w, h);

	return true;
}


bool AndroidVideoDeviceDriver::releaseContext() {
	// release the render context
	safe_release(render_context);

	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (context != EGL_NO_CONTEXT) {
			eglDestroyContext(display, context);
		}

		if (surface != EGL_NO_SURFACE) {
			eglDestroySurface(display, surface);
		}

		eglTerminate(display);
	}

	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
	config  = NULL;

	CHECK_GL_ERROR;

	return true;
}


bool AndroidVideoDeviceDriver::detachContext() {
	// detach the gl context
	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	}

	// release surface
	if (surface != EGL_NO_SURFACE) {
		eglDestroySurface(display, surface);
		surface = EGL_NO_SURFACE;
	}

	// the video device is suspended, no rendering should
	// be possible until the render context is re-attached
	setState(wiesel::video::Video_Suspended);
	
	return true;
}


bool AndroidVideoDeviceDriver::reattachContext() {
	return initContext();
}


void AndroidVideoDeviceDriver::onAndroidCommand(int32_t cmd) {
	switch(cmd) {
		case APP_CMD_CONFIG_CHANGED: {
			resize();
			break;
		}

		case APP_CMD_WINDOW_RESIZED: {
			resize();
			break;
		}

		case APP_CMD_INIT_WINDOW: {
			bool success = initContext();
			assert(success);
			break;
		}

		case APP_CMD_TERM_WINDOW: {
			// The window is being hidden or closed, clean it up.
			detachContext();
			break;
		}

		case APP_CMD_GAINED_FOCUS: {
			if (getScreen()) {
				getScreen()->getTouchHandler()->releaseAllTouches();
			}

			setState(wiesel::video::Video_Active);

			break;
		}

		case APP_CMD_LOST_FOCUS: {
			if (getScreen()) {
				getScreen()->getTouchHandler()->releaseAllTouches();
			}

			if (getState() == wiesel::video::Video_Active) {
				setState(wiesel::video::Video_Background);
			}

			break;
		}

		case APP_CMD_PAUSE:
		case APP_CMD_STOP: {
			setState(wiesel::video::Video_Suspended);
			break;
		}

		case APP_CMD_START:
		case APP_CMD_RESUME: {
			setState(wiesel::video::Video_Active);
			break;
		}

		case APP_CMD_DESTROY: {
			break;
		}
	}

	return;
}


int32_t AndroidVideoDeviceDriver::onAndroidInputEvent(AInputEvent *event) {
	wiesel::video::Screen *screen = getScreen();

	if (
			AInputEvent_getType(event)   == AINPUT_EVENT_TYPE_MOTION
		&&	AInputEvent_getSource(event) == AINPUT_SOURCE_TOUCHSCREEN
	) {
		int action = AMotionEvent_getAction(event);

		switch(action & AMOTION_EVENT_ACTION_MASK) {
			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN: {
				int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int id    = AMotionEvent_getPointerId(event, index);
				int x     = AMotionEvent_getX(event, index);
				int y     = AMotionEvent_getY(event, index);

				if (screen) {
					vector2d location = convertScreenToWorld(vector2d(x, y));
					screen->getTouchHandler()->startTouch(id, location.x, location.y);
				}

				break;
			}

			case AMOTION_EVENT_ACTION_MOVE: {
				for(int index=AMotionEvent_getPointerCount(event); --index>=0;) {
					int id    = AMotionEvent_getPointerId(event, index);
					int x     = AMotionEvent_getX(event, index);
					int y     = AMotionEvent_getY(event, index);

					if (screen) {
						vector2d location = convertScreenToWorld(vector2d(x, y));
						screen->getTouchHandler()->updateTouchLocation(id, location.x, location.y);
					}
				}

				break;
			}

			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_CANCEL:
			case AMOTION_EVENT_ACTION_POINTER_UP: {
				int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int id    = AMotionEvent_getPointerId(event, index);

				if (screen) {
					screen->getTouchHandler()->releaseTouch(id);
				}

				break;
			}
		}

		return 1;
	}

	return 0;
}


void AndroidVideoDeviceDriver::preRender() {
	if (display == EGL_NO_DISPLAY) {
		return;
	}

	OpenGlVideoDeviceDriver::preRender();

	if (render_context) {
		render_context->preRender();
		render_context->setProjectionMatrix(projection);
	}

	return;
}


void AndroidVideoDeviceDriver::postRender() {
	if (render_context) {
		render_context->postRender();
	}

	eglSwapBuffers(display, surface);
	CHECK_GL_ERROR;

	OpenGlVideoDeviceDriver::postRender();

	return;
}


RenderContext *AndroidVideoDeviceDriver::getCurrentRenderContext() {
	return render_context;
}
