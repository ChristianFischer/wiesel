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
#ifndef __WIESEL_DEVICE_RESOURCE_H__
#define __WIESEL_DEVICE_RESOURCE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/util/shared_object.h>



namespace wiesel {

	// forward declarations
	
	class Device;
	
	
	class WIESEL_CORE_EXPORT DeviceResource : public virtual SharedObject
	{
	private:
		/// hidden default constructor
		DeviceResource();
		
	protected:
		/// create a new device resource, 
		DeviceResource(Device *device);
		
	public:
		virtual ~DeviceResource();
		
	// getters
	public:
		/// get the device, which owns this resource
		inline Device* getDevice() {
			return device;
		}
		
		/// get the device, which owns this resource
		inline const Device* getDevice() const {
			return device;
		}
		
	private:
		Device*		device;
	};

}

#endif /* __WIESEL_DEVICE_RESOURCE_H__ */
