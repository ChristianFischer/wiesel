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

	return;
}

AndroidEngine::~AndroidEngine() {
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
			this->startApp();
		}
		else {
			screen->release();
			delete screen;
			this->requestExit();
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

    return false;
}


FileSystem *AndroidEngine::getRootFileSystem() {
	return root_fs;
}


FileSystem *AndroidEngine::getAssetFileSystem() {
	return asset_fs;
}


bool AndroidEngine::decodeImage(
		DataSource *data,
		unsigned char **pBuffer, size_t *pSize,
		unsigned int *pWidth, unsigned int *pHeight,
		unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
		int *pRbits, int *pGbits, int *pBbits, int *pAbits,
		bool as_texture
) {
	FileDataSource *filedata = dynamic_cast<FileDataSource*>(data);
	if (filedata) {
		string file_ext = filedata->getFile()->getExtension();
		// TODO: convert to lower case

		if (file_ext == "png") {
			return decodeImage_PNG(
					data, pBuffer, pSize,
					pWidth, pHeight, pOriginalWidth, pOriginalHeight,
					pRbits, pGbits, pBbits, pAbits, as_texture
			);
		}
	}
	else {
		DataBuffer *buffer = data->getDataBuffer();

		// check first bytes
		if (buffer->getSize() >= 4) {
			uint32_t header_32 = *(reinterpret_cast<const int32_t*>(buffer->getData()));

			// when a format was recognized, we create a new BufferDataSource, to re-use the created data-buffer,
			// because passing the original 'data' object could cause the buffer will be re-loaded from a storage.

			if (header_32 == 0x474e5089U) {
				return decodeImage_PNG(
						new BufferDataSource(buffer), pBuffer, pSize,
						pWidth, pHeight, pOriginalWidth, pOriginalHeight,
						pRbits, pGbits, pBbits, pAbits, as_texture
				);
			}
		}
	}

	return false;
}

#endif // __ANDROID__
