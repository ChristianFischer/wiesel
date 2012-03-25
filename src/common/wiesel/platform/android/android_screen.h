/*
 * screen.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_PLATFORM_ANDROID_SCREEN_H__
#define __WIESEL_PLATFORM_ANDROID_SCREEN_H__

#ifdef __ANDROID__

#include "../../screen.h"
#include "android_engine.h"

#include <wiesel/geometry.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android_native_app_glue.h>


namespace wiesel {

	/**
	 * @brief The android Screen implementation.
	 */
	class AndroidScreen
	: public Screen
	{
	private:
		AndroidScreen();

	public:
		AndroidScreen(AndroidEngine *engine, struct android_app *app);
		virtual ~AndroidScreen();

		bool init();
		bool release();

		virtual void preRender();
		virtual void postRender();

	private:
		struct android_app*	app;
		AndroidEngine*		engine;

	    EGLDisplay display;
	    EGLSurface surface;
	    EGLContext context;
	};

}

#endif // __ANDROID__

#endif /* __WIESEL_PLATFORM_ANDROID_SCREEN_H__ */
