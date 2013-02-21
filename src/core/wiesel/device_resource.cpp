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
#include "device_resource.h"
#include "device.h"

#include <assert.h>

using namespace wiesel;


DeviceResource::DeviceResource() {
	this->device		= NULL;
	this->is_requested	= false;
}

DeviceResource::~DeviceResource() {
	return;
}


void DeviceResource::_assign(Device *device) {
	if (getDevice() != device) {
		if (getDevice() != NULL) {
			getDevice()->removeResource(this);
		}

		if (device) {
			device->addResource(this);
		}
	}

	return;
}


void DeviceResource::setRequested(bool requested) {
	this->is_requested = requested;
}


bool DeviceResource::isRequested() const {
	return is_requested;
}


bool DeviceResource::loadContent() {
	setRequested(true);

	if (getDevice()) {
		doLoadContent();
	}

	return isLoaded();
}


void DeviceResource::releaseContent() {
	setRequested(false);
	doUnloadContent();
	return;
}




DeviceResourceContent::DeviceResourceContent() {
	return;
}

DeviceResourceContent::~DeviceResourceContent() {
	return;
}


