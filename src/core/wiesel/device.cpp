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
#include "device.h"
#include "device_resource.h"

#include <algorithm>

using namespace wiesel;


Device::Device() {
	return;
}


Device::~Device() {
	// free all device resources
	for(DeviceResourceList::iterator it=resources.begin(); it!=resources.end(); it++) {
		DeviceResource *resource = *it;

		// the content should already be unloaded
		assert(resource->isLoaded() == false);

		resource->release();
	}

	return;
}


void Device::addResource(DeviceResource *resource) {
	// check, if the resource is already in the list
	DeviceResourceList::iterator it = std::find(resources.begin(), resources.end(), resource);
	assert(it == resources.end());

	if (it == resources.end()) {
		resources.push_back(resource);
		resource->device = this;
		resource->retain();
	}

	return;
}


void Device::removeResource(DeviceResource *resource) {
	DeviceResourceList::iterator it = std::find(resources.begin(), resources.end(), resource);

	if ((*it) == resource) {
		resources.erase(it);

		if (resource->device == this) {
			resource->device = NULL;
		}

		(*it)->release();
	}

	return;
}


void Device::loadAllResources() {
	for(DeviceResourceList::iterator it=resources.begin(); it!=resources.end(); it++) {
		DeviceResource *resource = *it;
		resource->loadContent();
	}

	return;
}


void Device::unloadAllResources() {
	for(DeviceResourceList::iterator it=resources.begin(); it!=resources.end(); it++) {
		DeviceResource *resource = *it;
		resource->releaseContent();
	}

	return;
}




DeviceDriver::DeviceDriver() {
	return;
}


DeviceDriver::DeviceDriver(Device *device) {
	return;
}


DeviceDriver::~DeviceDriver() {
	return;
}
