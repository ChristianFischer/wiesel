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
#include "android_video_loader.h"
#include "android_video_driver.h"

#include <wiesel/video/video_loader.h>
#include <wiesel/engine.h>


using namespace wiesel;
using namespace wiesel::android;
using namespace wiesel::video;



AndroidVideoLoader::AndroidVideoLoader() {
	return;
}


AndroidVideoLoader::~AndroidVideoLoader() {
	return;
}


bool AndroidVideoLoader::loadVideoDevice(Screen *screen, const dimension &resolution, unsigned int flags) {
	// try to find the sdl platform
	AndroidPlatform *platform = NULL;
	const std::vector<Platform*> *platforms = Engine::getInstance()->getPlatforms();
	for(std::vector<Platform*>::const_iterator it=platforms->begin(); it!=platforms->end(); it++) {
		AndroidPlatform *android_platform = dynamic_cast<AndroidPlatform*>(*it);
		if (android_platform) {
			platform = android_platform;
		}
	}

	// sdl platform is required
	assert(platform);
	if (platform == NULL) {
		return false;
	}

	// create the new video device
	AndroidVideoDeviceDriver *device = new AndroidVideoDeviceDriver(platform, screen);

	// inizialize the window
	if (device->init() == false) {
		return false;
	}

	// try to install the device into the screen
	if (install(screen, device) == false) {
		return false;
	}

	return true;
}

