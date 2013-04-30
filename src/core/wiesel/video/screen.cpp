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
#include "screen.h"

#include "video_driver.h"
#include "video_loader.h"

#include "wiesel/ui/touchhandler.h"
#include "wiesel/module_registry.h"

using namespace wiesel;
using namespace wiesel::video;


Screen::Screen() {
	video_device_driver  = NULL;

	touch_handler = new TouchHandler();
	keep(touch_handler);

	return;
}

Screen::~Screen() {
	setVideoDeviceDriver(NULL);
	safe_release(touch_handler);

	return;
}


VideoState Screen::getState() const {
	if (getVideoDeviceDriver()) {
		return getVideoDeviceDriver()->getState();
	}

	return Video_Uninitialized;
}


bool Screen::loadVideoDevice(const dimension &resolution, unsigned int flags) {
	std::vector<ModuleLoader<IVideoLoader>*> loaders = ModuleRegistry::getInstance()->findModules<IVideoLoader>();
	for(std::vector<ModuleLoader<IVideoLoader>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		ref<IVideoLoader> loader = (*it)->create();
		if (loader == NULL) {
			continue;
		}

		bool success = loader->loadVideoDevice(this, resolution, flags);
		if (success) {
			return true;
		}
	}

	return false;
}


void Screen::setVideoDeviceDriver(VideoDeviceDriver *driver) {
	if (this->video_device_driver != driver) {
		if (this->video_device_driver) {

			// unload all resources
			this->unloadAllResources();

			release(this->video_device_driver);
			this->video_device_driver = NULL;
		}

		if (driver) {
			this->video_device_driver = driver;
			keep(this->video_device_driver);

			// load all resources from the new driver
			this->loadAllResources();
		}
	}

	return;
}

