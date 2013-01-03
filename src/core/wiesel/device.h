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
#ifndef __WIESEL_DEVICE_H__
#define __WIESEL_DEVICE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/util/shared_object.h>

#include <vector>


namespace wiesel {
	
	
	// forward declarations
	
	class DeviceResource;
	

	/**
	 * @brief An abstract baseclass for any device classes.
	 * Device classes wwill be used as an interface layer between application
	 * and various hardware and platform specific implementations, such as
	 * video devices for rendering or input devices for pointer and key input.
	 * A device can handle various \ref DeviceResource objects.
	 */
	class WIESEL_CORE_EXPORT Device : public virtual SharedObject
	{
	protected:
		Device();
		
	public:
		virtual ~Device();
		
	// handle resources
	protected:
		/**
		 * @brief Adds a new resource to this device.
		 * This is allowed for subclasses only. Subclasses should provide 
		 * their own set of functions to provide access to device resources.
		 */
		void addResource(DeviceResource *resource);
		
		/**
		 * @brief Removes an existing resource to this device.
		 * This is allowed for subclasses only. Subclasses should provide 
		 * their own set of functions to provide access to device resources.
		 */
		void removeResource(DeviceResource *resource);
		
	protected:
		/// type alias for resource lists
		typedef std::vector<DeviceResource*>	DeviceResourceList;
		
		DeviceResourceList		resources;
	};

}

#endif /* __WIESEL_ENGINE_H__ */
