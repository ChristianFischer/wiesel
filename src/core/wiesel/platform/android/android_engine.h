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
#ifndef __WIESEL_PLATFORM_ANDROID_ENGINE_H__
#define __WIESEL_PLATFORM_ANDROID_ENGINE_H__

#include <wiesel/wiesel-core.def>

#ifdef __ANDROID__

#include "../../../wiesel/engine.h"

#include <android_native_app_glue.h>


namespace wiesel {

	class GenericFileSystem;
	class AndroidAssetFileSystem;


	/**
	 * @brief The android engine implementation.
	 */
	class WIESEL_CORE_EXPORT AndroidEngine
	: public Engine
	{
	public:
		AndroidEngine(struct android_app *app);
		virtual ~AndroidEngine();

	// to be called by callback functions
	public:
		void __handle_cmd(struct android_app* app, int32_t cmd);

	protected:
		bool initWindow();
		void closeWindow();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();

		virtual TouchHandler *getTouchHandler();

	protected:
		struct android_app*			app;
		GenericFileSystem*			root_fs;
		AndroidAssetFileSystem*		asset_fs;
		TouchHandler*				touch_handler;
	};

}

#endif /* __ANDROID__ */
#endif /* __WIESEL_PLATFORM_ANDROID_ENGINE_H__ */
