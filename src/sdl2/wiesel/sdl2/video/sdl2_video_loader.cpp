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
#include "sdl2_video_loader.h"
#include "sdl2_video_driver.h"

#include <wiesel/video/video_loader.h>
#include <wiesel/engine.h>


using namespace wiesel;
using namespace wiesel::sdl2;
using namespace wiesel::video;



Sdl2VideoLoader::Sdl2VideoLoader() {
	return;
}


Sdl2VideoLoader::~Sdl2VideoLoader() {
	return;
}


Sdl2VideoLoader *Sdl2VideoLoader::create() {
	return new Sdl2VideoLoader();
}


bool Sdl2VideoLoader::loadVideoDevice(wiesel::video::Screen *screen, const dimension &resolution, unsigned int flags) {
	// try to find the sdl platform
	Sdl2Platform *platform = NULL;
	const std::vector<Platform*> *platforms = Engine::getInstance()->getPlatforms();
	for(std::vector<Platform*>::const_iterator it=platforms->begin(); it!=platforms->end(); it++) {
		Sdl2Platform *sdl_platform = dynamic_cast<Sdl2Platform*>(*it);
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
	Sdl2VideoDeviceDriver *device_driver = new Sdl2VideoDeviceDriver(platform, screen);

	// inizialize the window
	if (device_driver->init(resolution, flags) == false) {
		return false;
	}

	// try to install the device into the screen
	if (install(screen, device_driver) == false) {
		return false;
	}

	return true;
}

