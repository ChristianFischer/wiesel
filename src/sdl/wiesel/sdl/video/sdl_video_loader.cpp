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
#include "sdl_video_loader.h"
#include "sdl_video_device.h"

#include <wiesel/video/video_loader.h>
#include <wiesel/engine.h>


using namespace wiesel;
using namespace wiesel::sdl;
using namespace wiesel::video;



SdlVideoLoader::SdlVideoLoader() {
	return;
}


SdlVideoLoader::~SdlVideoLoader() {
	return;
}


SdlVideoLoader *SdlVideoLoader::create() {
	return new SdlVideoLoader();
}


bool SdlVideoLoader::loadVideoDevice(Screen *screen, const dimension &resolution, unsigned int flags) {
	// try to find the sdl platform
	SdlPlatform *platform = NULL;
	const std::vector<Platform*> *platforms = Engine::getInstance()->getPlatforms();
	for(std::vector<Platform*>::const_iterator it=platforms->begin(); it!=platforms->end(); it++) {
		SdlPlatform *sdl_platform = dynamic_cast<SdlPlatform*>(*it);
		if (sdl_platform) {
			platform = sdl_platform;
		}
	}

	// sdl platform is required
	assert(platform);
	if (platform == NULL) {
		return false;
	}

	// create the new video device
	SdlVideoDevice *device = new SdlVideoDevice(platform, screen);

	// inizialize the window
	if (device->init(resolution, flags) == false) {
		return false;
	}

	// try to install the device into the screen
	if (install(screen, device) == false) {
		return false;
	}

	return true;
}

