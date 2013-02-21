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
#include "video_loader.h"
#include "video_driver.h"
#include "screen.h"

using namespace wiesel;
using namespace wiesel::video;


IVideoLoader::IVideoLoader() {
	return;
}


IVideoLoader::~IVideoLoader() {
	return;
}


bool IVideoLoader::install(Screen *screen, VideoDeviceDriver *driver) {
	assert(screen);

	// cannot install a new device into a screen, which already contains a video device
	assert(screen->getVideoDeviceDriver() == NULL);

	if (screen && screen->getVideoDeviceDriver()==NULL) {
		screen->setVideoDeviceDriver(driver);
		return true;
	}

	return false;
}

