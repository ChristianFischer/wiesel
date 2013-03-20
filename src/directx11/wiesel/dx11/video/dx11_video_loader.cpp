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
#include "dx11_video_loader.h"
#include "dx11_video_driver.h"

#include <wiesel/video/video_loader.h>
#include <wiesel/engine.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;



Dx11VideoLoader::Dx11VideoLoader() {
	return;
}


Dx11VideoLoader::~Dx11VideoLoader() {
	return;
}


bool Dx11VideoLoader::loadVideoDevice(wiesel::video::Screen *screen, const dimension &resolution, unsigned int flags) {
	// create the new video device
	Dx11VideoDeviceDriver *device_driver = new Dx11VideoDeviceDriver(screen);

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

