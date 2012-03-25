/*
 *  Created on: 19.03.2012
 *      Author: Baldur
 */
#ifdef __ANDROID__

#include "platform_main.h"
#include "android_engine.h"
#include "android_screen.h"

#include <android_native_app_glue.h>


using namespace wiesel;


void __internal_android_main(Application *app, struct ::android_app* state) {
	// Make sure glue isn't stripped.
	app_dummy();

	// create the android engine object
	AndroidEngine *engine = new AndroidEngine(state);
	Engine::install(engine);
	Engine::run(app);
	Engine::shutdown();

	return;
}


#endif // __ANDROID__
