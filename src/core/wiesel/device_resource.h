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

#include "device.h"


namespace wiesel {


	/**
	 * @brief A base class for resources like textures or sounds.
	 * The resource manages the basic interface and settings for the resource,
	 * but doesn't store any data. The device-specific data will be stored
	 * in a separate ResourceContent object.
	 */
	class WIESEL_CORE_EXPORT DeviceResource : public virtual SharedObject
	{
	friend class Device;

	protected:
		/// create a new device resource, 
		DeviceResource();
		
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

	// protected setters
	protected:
		/**
		 * @brief Assigns this resource to a new device.
		 * This function has to be used in subclasses only. Each subclass should provide it's own
		 * assign-function to ensure, the given device is appropriate for the specific resource.
		 */
		void _assign(Device *device);

	public:
		/**
		 * @brief Set a request for this resource, it should load it's content
		 * and prepare itself for usage.
		 * Changing the \c requested flag does not load or unload the resource directly.
		 * For loading the resource's content, a loading task will be required.
		 */
		void setRequested(bool requested);

		/**
		 * @brief Checks, if the resource's content was requested.
		 * This doesn't mean the content was already loaded and is ready for usage.
		 */
		bool isRequested() const;

		/**
		 * @brief Immediately loads the content of this resource.
		 * This function blocks until the resource was fully loaded and does not provide
		 * any progress information during loading.
		 * The resource needs to be attached to a device in order to be loaded.
		 * Calling this function sets the 'requested' flag, even if loading fails.
		 * @returns \c true when the resource was successfully loaded.
		 */
		bool loadContent();

		/**
		 * @brief Releases the resource's content.
		 * The resource is no longer usable after cleanup was called.
		 * After this function was completed, the 'requested' flag is set to \c false.
		 */
		void releaseContent();

		/**
		 * @brief Checks, if the resource's content was loaded and is ready to be used.
		 */
		virtual bool isLoaded() const = 0;

	protected:
		/**
		 * @brief A callback function which will be invoked, when the resource should load it's content.
		 * This function needs to be implemented by subclasses.
		 */
		virtual bool doLoadContent() = 0;

		/**
		 * @brief A callback function which will be invoked, when the resource should free it's content.
		 * This function needs to be implemented by subclasses.
		 */
		virtual bool doUnloadContent() = 0;
		
	private:
		Device*		device;

		bool		is_requested;
	};



	/**
	 * @brief A base template class for creating own device resources with specific content classes.
	 */
	template <class DeviceClass, class ContentClass>
	class TDeviceResource : public DeviceResource
	{
	protected:
		TDeviceResource() {
			this->content = NULL;
			return;
		}

	public:
		virtual ~TDeviceResource() {
			assert(getContent() == NULL);
			setContent(NULL);
			return;
		}

	public:
		/// assigns this resource to a new device
		virtual void assign(DeviceClass *device) {
			_assign(device);
		}


		/// assigns this device to a new device and afterwards loads the resource's content from it
		void loadContentFrom(DeviceClass *device) {
			if (device) {
				assign(device);
				loadContent();
			}

			return;
		}


		virtual bool isLoaded() const {
			return (this->content != NULL);
		}

	public:
		/// get the assigned content object
		inline ContentClass *getContent() {
			return content;
		}

		/// get the assigned content object
		inline const ContentClass *getContent() const {
			return content;
		}

	protected:
		/// set the content object for the current resource.
		void setContent(ContentClass *content) {
			if (this->content != content) {
				if (this->content) {
					this->content->release();
					this->content = NULL;
				}

				if (content) {
					this->content = content;
					this->content->retain();
				}
			}
		}

	private:
		ContentClass*	content;
	};



	/**
	 * @brief A class managing the content of a DeviceResource object.
	 * This class contains the device specific part.
	 */
	class WIESEL_CORE_EXPORT DeviceResourceContent : public virtual SharedObject
	{
	protected:
		DeviceResourceContent();

	public:
		virtual ~DeviceResourceContent();
	};
}

#endif /* __WIESEL_DEVICE_RESOURCE_H__ */
