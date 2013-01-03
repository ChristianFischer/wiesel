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

#include "video_device.h"
#include "video_loader.h"

#include "wiesel/ui/touchhandler.h"
#include "wiesel/module_registry.h"

using namespace wiesel;
using namespace wiesel::video;


Screen::Screen() {
	video_device  = NULL;

	touch_handler = new TouchHandler();
	touch_handler->retain();

	return;
}

Screen::~Screen() {
	if (video_device) {
		video_device->release();
		video_device = NULL;
	}

	if (touch_handler) {
		touch_handler->release();
	}

	return;
}


VideoState Screen::getState() const {
	if (getVideoDevice()) {
		return getVideoDevice()->getState();
	}

	return Video_Uninitialized;
}


bool Screen::loadVideoDevice(const dimension &resolution, unsigned int flags) {
	std::vector<ModuleLoader<IVideoLoader>*> loaders = ModuleRegistry::getInstance()->findModules<IVideoLoader>();
	for(std::vector<ModuleLoader<IVideoLoader>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		IVideoLoader *loader = (*it)->create();
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

