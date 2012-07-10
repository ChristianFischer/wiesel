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
#ifndef __WIESEL_PLATFORM_ANDROID_SCREEN_H__
#define __WIESEL_PLATFORM_ANDROID_SCREEN_H__

#ifdef __ANDROID__

#include "../../screen.h"
#include "android_engine.h"

#include <wiesel/geometry.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

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
		bool initContext();
		bool resize();
		bool detachContext();
		bool reattachContext();
		bool releaseContext();

		virtual void preRender();
		virtual void postRender();

	private:
		struct android_app*	app;
		AndroidEngine*		engine;

		EGLint     format;
		EGLConfig  config;
	    EGLDisplay display;
	    EGLSurface surface;
	    EGLContext context;
	};

}

#endif // __ANDROID__

#endif /* __WIESEL_PLATFORM_ANDROID_SCREEN_H__ */
