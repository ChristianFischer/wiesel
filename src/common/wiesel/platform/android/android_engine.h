/*
 * engine.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_PLATFORM_ANDROID_ENGINE_H__
#define __WIESEL_PLATFORM_ANDROID_ENGINE_H__

#ifdef __ANDROID__

#include "../../../wiesel/engine.h"

#include <android_native_app_glue.h>


namespace wiesel {

	/**
	 * @brief The android engine implementation.
	 */
	class AndroidEngine
	: public Engine
	{
	public:
		AndroidEngine(struct android_app *app);
		virtual ~AndroidEngine();

	// to be called by callback functions
	public:
		void __cmd_init_window();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual bool onRun();

	protected:
		struct android_app*		app;
	};

}

#endif /* __ANDROID__ */
#endif /* __WIESEL_PLATFORM_ANDROID_ENGINE_H__ */
