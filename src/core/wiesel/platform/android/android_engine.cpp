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

#include "android_engine.h"
#include "android_screen.h"

#include "wiesel/platform/generic/generic_root_fs.h"
#include "wiesel/platform/android/file_support/android_asset_fs.h"

#include "wiesel/io/datasource.h"
#include "wiesel/io/file.h"
#include "wiesel/ui/touchhandler.h"
#include "wiesel/util/log.h"

#include <assert.h>
#include <inttypes.h>
#include <android_native_app_glue.h>


using namespace wiesel;
using namespace std;


AndroidEngine::AndroidEngine(struct android_app *app) {
	this->app = app;

	// create file systems
	root_fs		= new GenericFileSystem();
	asset_fs	= new AndroidAssetFileSystem(app->activity->assetManager);

	// create touch handler object and register as updateable
	touch_handler = new TouchHandler();
	touch_handler->retain();
	registerUpdateable(touch_handler);

	return;
}

AndroidEngine::~AndroidEngine() {
	assert(screen == NULL);

	unregisterUpdateable(touch_handler);
	safe_release(touch_handler);

	delete root_fs;
	delete asset_fs;

	return;
}






/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    AndroidEngine *engine = (AndroidEngine*)app->userData;
    assert(engine->isActive());

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

				engine->getTouchHandler()->startTouch(id, x, y);

				break;
			}

			case AMOTION_EVENT_ACTION_MOVE: {
				for(int index=AMotionEvent_getPointerCount(event); --index>=0;) {
					int id    = AMotionEvent_getPointerId(event, index);
					int x     = AMotionEvent_getX(event, index);
					int y     = AMotionEvent_getY(event, index);

					engine->getTouchHandler()->updateTouchLocation(id, x, y);
				}

				break;
			}

			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_CANCEL:
			case AMOTION_EVENT_ACTION_POINTER_UP: {
				int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int id    = AMotionEvent_getPointerId(event, index);

				engine->getTouchHandler()->releaseTouch(id);

				break;
			}
		}

		return 1;
    }

    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	AndroidEngine* engine = static_cast<AndroidEngine*>(app->userData);
	if (engine) {
		engine->__handle_cmd(app, cmd);
	}

	return;
}








bool AndroidEngine::onInit() {
    app->userData     = this;
    app->onAppCmd     = engine_handle_cmd;
    app->onInputEvent = engine_handle_input;
	return true;
}


void AndroidEngine::__handle_cmd(android_app* app, int32_t cmd) {
	/*
	string cmd_name = "<unknown>";
	switch(cmd) {
		case APP_CMD_INPUT_CHANGED:				cmd_name = "APP_CMD_INPUT_CHANGED";				break;
		case APP_CMD_INIT_WINDOW:				cmd_name = "APP_CMD_INIT_WINDOW";				break;
		case APP_CMD_TERM_WINDOW:				cmd_name = "APP_CMD_TERM_WINDOW";				break;
		case APP_CMD_WINDOW_RESIZED:			cmd_name = "APP_CMD_WINDOW_RESIZED";			break;
		case APP_CMD_WINDOW_REDRAW_NEEDED:		cmd_name = "APP_CMD_WINDOW_REDRAW_NEEDED";		break;
		case APP_CMD_CONTENT_RECT_CHANGED:		cmd_name = "APP_CMD_CONTENT_RECT_CHANGED";		break;
		case APP_CMD_GAINED_FOCUS:				cmd_name = "APP_CMD_GAINED_FOCUS";				break;
		case APP_CMD_LOST_FOCUS:				cmd_name = "APP_CMD_LOST_FOCUS";				break;
		case APP_CMD_CONFIG_CHANGED:			cmd_name = "APP_CMD_CONFIG_CHANGED";			break;
		case APP_CMD_LOW_MEMORY:				cmd_name = "APP_CMD_LOW_MEMORY";				break;
		case APP_CMD_START:						cmd_name = "APP_CMD_START";						break;
		case APP_CMD_RESUME:					cmd_name = "APP_CMD_RESUME";					break;
		case APP_CMD_SAVE_STATE:				cmd_name = "APP_CMD_SAVE_STATE";				break;
		case APP_CMD_PAUSE:						cmd_name = "APP_CMD_PAUSE";						break;
		case APP_CMD_STOP:						cmd_name = "APP_CMD_STOP";						break;
		case APP_CMD_DESTROY:					cmd_name = "APP_CMD_DESTROY";					break;
	}

	Log::info << endl << cmd_name << endl;
	*/

	switch (cmd) {
		case APP_CMD_SAVE_STATE: {
			// The system has asked us to save our current state.  Do so.
			break;
		}

		case APP_CMD_INIT_WINDOW: {
			// The window is being shown, get it ready.
			if (initWindow() == false) {
				requestExit();
			}

			break;
		}

		case APP_CMD_CONFIG_CHANGED: {
			AndroidScreen *screen = dynamic_cast<AndroidScreen*>(getScreen());
			if (screen) {
				screen->resize();
			}

			break;
		}

		case APP_CMD_WINDOW_RESIZED: {
			AndroidScreen *screen = dynamic_cast<AndroidScreen*>(getScreen());
			if (screen) {
				screen->resize();
			}

			break;
		}

		case APP_CMD_TERM_WINDOW: {
			// The window is being hidden or closed, clean it up.
			AndroidScreen *android_screen = dynamic_cast<AndroidScreen*>(screen);
			if (android_screen) {
				android_screen->detachContext();
			}

			break;
		}

		case APP_CMD_GAINED_FOCUS: {
			getTouchHandler()->releaseAllTouches();
			enterForeground();
			break;
		}

		case APP_CMD_LOST_FOCUS: {
			getTouchHandler()->releaseAllTouches();
			enterBackground();
			break;
		}

		case APP_CMD_PAUSE:
		case APP_CMD_STOP: {
			suspendApp();
			break;
		}

		case APP_CMD_START:
		case APP_CMD_RESUME: {
			resumeSuspendedApp();
			break;
		}

		case APP_CMD_DESTROY: {
			closeWindow();

			if (isActive()) {
				Engine::requestExit();
			}

			break;
		}
	}

	return;
}


bool AndroidEngine::initWindow() {
	if (app->window != NULL) {
		// when there's still an existing screen, try to re-attach to it's EGL context
		if (screen != NULL) {
			AndroidScreen *android_screen = dynamic_cast<AndroidScreen*>(screen);
			bool success = false;

			if (android_screen && android_screen->reattachContext()) {
				success = true;
			}

			if (!success) {
				closeWindow();
				assert(screen == NULL);
			}
		}

		if (screen == NULL) {
			AndroidScreen *screen = new AndroidScreen(this, app);
			if (screen->init()) {
				this->screen = screen;
				this->startApp();
			}
			else {
				screen->releaseContext();
				delete screen;
				this->requestExit();
			}
		}
	}

	return true;
}


void AndroidEngine::closeWindow() {
	AndroidScreen *android_screen = dynamic_cast<AndroidScreen*>(screen);
	if (android_screen) {
		android_screen->releaseContext();
	}

	if (screen) {
		delete screen;
		screen = NULL;
	}

	return;
}


void AndroidEngine::onShutdown() {
	closeWindow();

	// close the java activity
	ANativeActivity_finish(app->activity);

	return;
}


void AndroidEngine::onRunFirst() {
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

	switch(getState()) {
		case Engine_Background:
		case Engine_Suspended: {
			// no need to waste CPU time while in suspend state...
			sleep(1);
			break;
		}

		default: {
			break;
		}
	}

    return false;
}


FileSystem *AndroidEngine::getRootFileSystem() {
	return root_fs;
}


FileSystem *AndroidEngine::getAssetFileSystem() {
	return asset_fs;
}


TouchHandler *AndroidEngine::getTouchHandler() {
	return touch_handler;
}

#endif // __ANDROID__
