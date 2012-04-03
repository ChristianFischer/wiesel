/*
 * screen.cpp
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#include "android_screen.h"
#include "log.h"

#include <wiesel/gl/gl.h>
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
{
	return;
}


AndroidScreen::~AndroidScreen() {
	// check if the screen was released correctly
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);
	return;
}


bool AndroidScreen::init() {
	// check if the screen is not already initialized
	assert(display == EGL_NO_DISPLAY);
	assert(surface == EGL_NO_SURFACE);
	assert(context == EGL_NO_CONTEXT);

	// initialize OpenGL ES and EGL

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

	const EGLint context_attribs[] = {
			EGL_CONTEXT_CLIENT_VERSION,		2,
			EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig  config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, config_attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, app->window, NULL);
	context = eglCreateContext(display, config, NULL, context_attribs);
	CHECK_GL_ERROR;

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGE("eglMakeCurrent failed!");
		return false;
	}

	// get the display size
	eglQuerySurface(display, surface, EGL_WIDTH,  &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	CHECK_GL_ERROR;

	// store the created values
	this->display = display;
	this->context = context;
	this->surface = surface;
	this->size    = dimension(w, h);

	// Initialize GL state.
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// setup viewport
	glViewport(0, 0, w, h);

	// log OpenGL information
	LOGI("OpenGL Version:    %s", ((const char*)glGetString(GL_VERSION)));
	LOGI("OpenGL Vendor:     %s", ((const char*)glGetString(GL_VENDOR)));
	LOGI("OpenGL Renderer:   %s", ((const char*)glGetString(GL_RENDERER)));
	LOGI("OpenGL Shader:     %s", ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	LOGI("OpenGL Extensions: %s", ((const char*)glGetString(GL_EXTENSIONS)));

	CHECK_GL_ERROR;

	return true;
}


bool AndroidScreen::release() {
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

	CHECK_GL_ERROR;

	return true;
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

