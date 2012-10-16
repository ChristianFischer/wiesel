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
#if WIESEL_USE_LIBSDL

#include "sdl_engine.h"
#include "sdl_screen.h"

#include "wiesel/platform/generic/generic_root_fs.h"

#include "wiesel/io/datasource.h"
#include "wiesel/io/directory_filesystem.h"
#include "wiesel/util/log.h"
#include "wiesel/ui/touchhandler.h"

#include <SDL_image.h>

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>
#include <sys/unistd.h>
#include <sys/param.h>
#include <sstream>


using namespace wiesel;
using namespace std;


SdlEngine::SdlEngine() {
	// create file systems
	root_fs = new GenericFileSystem();

	// get the current working directory
	char working_dir_name[MAXPATHLEN];
	getcwd(working_dir_name, MAXPATHLEN);

	// replace backslashes with normal slashes, as the file system API
	// cannot handle backslashes as separators yet.
	for(int i=0; i<MAXPATHLEN; i++) {
		if (working_dir_name[i] == '\0') {
			break;
		}

		if (working_dir_name[i] == '\\') {
			working_dir_name[i] =  '/';
		}
	}

	stringstream ss;
	ss << working_dir_name;
	ss << "/resources/common";

	Directory *asset_root = root_fs->findDirectory(ss.str());
	assert(asset_root);

	if (asset_root) {
		asset_fs = new DirectoryFileSystem(asset_root);
	}
	else {
		// try another root file-system as a fallback
		asset_fs = new GenericFileSystem();
	}

	// create touch handler object and register as updateable
	touch_handler = new TouchHandler();
	touch_handler->retain();
	registerUpdateable(touch_handler);

	return;
}

SdlEngine::~SdlEngine() {
	unregisterUpdateable(touch_handler);
	safe_release(touch_handler);

	delete asset_fs;
	delete root_fs;

	return;
}


bool SdlEngine::onInit() {
	return true;
}


void SdlEngine::onShutdown() {
	if (screen) {
		static_cast<SdlScreen*>(screen)->release();
		delete screen;
		screen = NULL;
	}

	return;
}


void SdlEngine::onRunFirst() {
	assert(this->screen == NULL);

	if (this->screen == NULL) {
		SdlScreen *screen = new SdlScreen(this);
		if (screen->init()) {
			this->screen = screen;
			this->startApp();
			this->enterForeground();
		}
		else {
			screen->release();
			delete screen;
			this->requestExit();
		}
	}

	return;
}


bool SdlEngine::onRun() {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_MOUSEBUTTONDOWN: {
				getTouchHandler()->startTouch(event.button.button, event.button.x, event.button.y);
				break;
			}

			case SDL_MOUSEMOTION: {
				for(int button=0; button<5; button++) {
					getTouchHandler()->updateTouchLocation(button, event.motion.x, event.motion.y);
				}

				break;
			}

			case SDL_MOUSEBUTTONUP: {
				getTouchHandler()->releaseTouch(event.button.button);
				break;
			}

			case SDL_QUIT: {
				return true;
			}

			default: {
				break;
			}
		}
	}

	return false;
}


FileSystem *SdlEngine::getRootFileSystem() {
	return root_fs;
}


FileSystem *SdlEngine::getAssetFileSystem() {
	return asset_fs;
}


TouchHandler *SdlEngine::getTouchHandler() {
	return touch_handler;
}

#endif // WIESEL_USE_LIBSDL
