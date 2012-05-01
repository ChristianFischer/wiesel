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

#include <assert.h>
#include <inttypes.h>
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

	return;
}

SdlEngine::~SdlEngine() {
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
	return;
}


bool SdlEngine::onRun() {
	assert(this->screen == NULL);
	
	if (this->screen == NULL) {
		SdlScreen *screen = new SdlScreen(this);
		if (screen->init()) {
			this->screen = screen;
			this->startApp();
		}
		else {
			screen->release();
			delete screen;
			this->requestExit();
			return true;
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


bool SdlEngine::decodeImage(
		DataSource *data,
		unsigned char **pBuffer, size_t *pSize,
		unsigned int *pWidth, unsigned int *pHeight,
		unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
		int *pRbits, int *pGbits, int *pBbits, int *pAbits,
		bool as_texture
) {
	// TODO: implement me
	return false;
}

#endif // WIESEL_USE_LIBSDL
