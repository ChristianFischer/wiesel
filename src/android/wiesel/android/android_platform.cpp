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

#include "android_platform.h"
#include "io/android_asset_fs.h"
#include "video/android_video_device.h"

#include "wiesel/io/datasource.h"
#include "wiesel/io/file.h"
#include "wiesel/ui/touchhandler.h"
#include "wiesel/util/log.h"
#include "wiesel/video/video_device.h"
#include "wiesel/engine.h"

#include <algorithm>
#include <assert.h>
#include <inttypes.h>
#include <android_native_app_glue.h>


using namespace wiesel;
using namespace wiesel::android;
using namespace std;


AndroidPlatform::AndroidPlatform() {
	this->app					= NULL;
	this->asset_fs				= NULL;
	this->window_initialized	= false;

	return;
}

AndroidPlatform::~AndroidPlatform() {
	if (asset_fs) {
		delete asset_fs;
		asset_fs = NULL;
	}

	return;
}






/**
 * Process the next input event.
 */
int32_t wiesel::android::platform_handle_input(struct android_app* app, AInputEvent* event) {
	AndroidPlatform* platform = static_cast<AndroidPlatform*>(app->userData);
	if (platform) {
		return platform->onInputEvent(app, event);
	}

	return 0;
}

/**
 * Process the next main command.
 */
void wiesel::android::platform_handle_cmd(struct android_app* app, int32_t cmd) {
	AndroidPlatform* platform = static_cast<AndroidPlatform*>(app->userData);
	if (platform) {
		platform->onCommand(app, cmd);
	}

	return;
}








void AndroidPlatform::initAndroidApp(struct android_app *app) {
	this->app		= app;
	this->asset_fs	= new AndroidAssetFileSystem(app->activity->assetManager);

	app->userData     = this;
	app->onAppCmd     = platform_handle_cmd;
	app->onInputEvent = platform_handle_input;

	// wait until window was created
	do {
		onRun();
	}
	while(!window_initialized);

	return;
}


void AndroidPlatform::addReceiver(IAndroidMessageReceiver* recv) {
	receivers.push_back(recv);
	return;
}

void AndroidPlatform::removeReceiver(const IAndroidMessageReceiver* recv) {
	std::vector<IAndroidMessageReceiver*>::iterator it;

	while((it = std::find(receivers.begin(), receivers.end(), recv)) != receivers.end()) {
		receivers.erase(it);
	}

	return;
}


bool AndroidPlatform::onInit() {
	return true;
}


void AndroidPlatform::onCommand(android_app* app, int32_t cmd) {
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
	

	// check, if the app object is valid
	assert(app == this->app);
	if (app != this->app) {
		return;
	}

	switch (cmd) {
		case APP_CMD_SAVE_STATE: {
			// The system has asked us to save our current state.  Do so.
			break;
		}

		case APP_CMD_INIT_WINDOW: {
			// The window is being shown, get it ready.
			window_initialized = true;
			break;
		}

		case APP_CMD_CONFIG_CHANGED: {
			break;
		}

		case APP_CMD_WINDOW_RESIZED: {
			break;
		}

		case APP_CMD_TERM_WINDOW: {
			window_initialized = false;
			break;
		}

		case APP_CMD_GAINED_FOCUS: {
			break;
		}

		case APP_CMD_LOST_FOCUS: {
			break;
		}

		case APP_CMD_PAUSE:
		case APP_CMD_STOP: {
			break;
		}

		case APP_CMD_START:
		case APP_CMD_RESUME: {
			break;
		}

		case APP_CMD_DESTROY: {
			break;
		}
	}

	// send this event to all registered message receivers
	for(std::vector<IAndroidMessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
		(*it)->onAndroidCommand(cmd);
	}

	return;
}


int32_t AndroidPlatform::onInputEvent(struct android_app* app, AInputEvent* event) {
	int32_t ret = 0;

	// check, if the app object is valid
	assert(app == this->app);
	if (app != this->app) {
		return 0;
	}

	// send this event to all registered message receivers
	for(std::vector<IAndroidMessageReceiver*>::iterator it=receivers.begin(); it!=receivers.end(); it++) {
		int32_t r = (*it)->onAndroidInputEvent(event);
		if (r) {
			ret = r;
		}
	}

	return ret;
}


/*
bool AndroidPlatform::initWindow() {
	if (app->window != NULL) {
		// when there's still an existing screen, try to re-attach to it's EGL context
		if (video_device != NULL) {
			bool success = false;

			if (video_device->reattachContext()) {
				success = true;
			}

			if (!success) {
				closeWindow();
				assert(screen == NULL);
			}
		}

		if (video_device == NULL) {
			video_device = new AndroidVideoDevice(this, app);
			video_device->retain();

			if (video_device->init() == false) {
				closeWindow();
			}
		}
	}

	return video_device != NULL;
}


void AndroidPlatform::closeWindow() {
	if (video_device) {
		video_device->releaseContext();
		video_device->release();
		video_device = NULL;
	}

	return;
}
*/


void AndroidPlatform::onShutdown() {
	// close the java activity
	ANativeActivity_finish(app->activity);
	return;
}


void AndroidPlatform::onRunFirst() {
	return;
}


bool AndroidPlatform::onRun() {
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
			Engine::getInstance()->requestExit();
			return true;
		}
	}

	/*
	if (video_device) {
		switch(video_device->getState()) {
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
	}
	*/

	return false;
}


FileSystem *AndroidPlatform::getRootFileSystem() {
	return NULL;
}


FileSystem *AndroidPlatform::getAssetFileSystem() {
	return asset_fs;
}



IAndroidMessageReceiver::IAndroidMessageReceiver() {
	return;
}

IAndroidMessageReceiver::~IAndroidMessageReceiver() {
	return;
}


#endif // __ANDROID__
