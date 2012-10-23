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
#ifdef __ANDROID__

#include "android_screen.h"
#include <wiesel/gl/gl.h>
#include <wiesel/util/log.h>
#include <assert.h>


using namespace wiesel;



AndroidScreen::AndroidScreen()
: engine(NULL)
, app(NULL)
, display(EGL_NO_DISPLAY)
, surface(EGL_NO_SURFACE)
, context(EGL_NO_CONTEXT)
{
	return;
}


AndroidScreen::AndroidScreen(AndroidEngine *engine, struct android_app *app)
: engine(engine)
, app(app)
, display(EGL_NO_DISPLAY)
, surface(EGL_NO_SURFACE)
, context(EGL_NO_CONTEXT)
, config (NULL)
{
	return;
}


AndroidScreen::~AndroidScreen() {
	// check if the screen was released correctly
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);
	assert(config  == NULL);
	return;
}


bool AndroidScreen::init() {
	// check if the screen is not already initialized
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);
	assert(config  == NULL);

	if (!initContext()) {
		return false;
	}

	// log OpenGL information
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Version:    %s", ((const char*)glGetString(GL_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Vendor:     %s", ((const char*)glGetString(GL_VENDOR)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Renderer:   %s", ((const char*)glGetString(GL_RENDERER)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Shader:     %s", ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	logmsg(LogLevel_Info, WIESEL_GL_LOG_TAG, "OpenGL Extensions: %s", ((const char*)glGetString(GL_EXTENSIONS)));

	CHECK_GL_ERROR;

	return true;
}

bool AndroidScreen::initContext() {
	// initialize OpenGL ES and EGL

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

	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize the window's size
	resize();

	return true;
}


bool AndroidScreen::resize() {
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


bool AndroidScreen::releaseContext() {
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


bool AndroidScreen::detachContext() {
	// detach the gl context
	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	}

	// release surface
	if (surface != EGL_NO_SURFACE) {
		eglDestroySurface(display, surface);
		surface = EGL_NO_SURFACE;
	}
	
	return true;
}


bool AndroidScreen::reattachContext() {
	return initContext();
}


void AndroidScreen::preRender() {
	if (display == EGL_NO_DISPLAY) {
		return;
	}

	// Just fill the screen with a color.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	CHECK_GL_ERROR;

	return;
}


void AndroidScreen::postRender() {
	eglSwapBuffers(display, surface);
	CHECK_GL_ERROR;
	return;
}

#endif // __ANDROID__
