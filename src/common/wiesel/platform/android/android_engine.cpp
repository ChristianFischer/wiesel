/*
 * engine.cpp
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */
#ifdef __ANDROID__

#include "android_engine.h"
#include "android_screen.h"

#include <assert.h>
#include <android_native_app_glue.h>


using namespace wiesel;


AndroidEngine::AndroidEngine(struct android_app *app) {
	this->app = app;
	return;
}

AndroidEngine::~AndroidEngine() {
	return;
}






/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    AndroidEngine *engine = (AndroidEngine*)app->userData;
    assert(engine->isActive());

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int x = AMotionEvent_getX(event, 0);
        int y = AMotionEvent_getY(event, 0);

        // TODO: Handle Pointer Event

        return 1;
    }

    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	AndroidEngine* engine = static_cast<AndroidEngine*>(app->userData);

	switch (cmd) {
		case APP_CMD_SAVE_STATE: {
			// The system has asked us to save our current state.  Do so.
			break;
		}

		case APP_CMD_INIT_WINDOW: {
			// The window is being shown, get it ready.
			engine->__cmd_init_window();
			break;
		}

		case APP_CMD_TERM_WINDOW: {
			// The window is being hidden or closed, clean it up.
			if (engine->isActive()) {
				Engine::requestExit();
			}

			break;
		}

		case APP_CMD_GAINED_FOCUS: {
			break;
		}

		case APP_CMD_LOST_FOCUS: {
			break;
		}
	}

	return;
}








bool AndroidEngine::onInit() {
    app->userData     = this;
    app->onAppCmd     = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;
	return true;
}


void AndroidEngine::__cmd_init_window() {
	if (app->window != NULL) {
		AndroidScreen *screen = new AndroidScreen(this, app);
		if (screen->init()) {
			this->screen = screen;
		}
		else {
			screen->release();
			delete screen;
		}
	}

	return;
}


void AndroidEngine::onShutdown() {
	if (screen) {
		static_cast<AndroidScreen*>(screen)->release();
		delete screen;
		screen = NULL;
	}

	return;
}


bool AndroidEngine::onRun() {
	// Read all pending events.
	int ident;
	int events;
	struct android_poll_source* source;

	// loop until all events are read, then continue
	while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
		// Process this event.
		if (source != NULL) {
			source->process(app, source);
		}

		// Check if we are exiting.
		if (app->destroyRequested != 0) {
			Engine::requestExit();
			return true;
		}
	}

    return false;
}

#endif // __ANDROID__
