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
#ifndef __WIESEL_VIDEO_SCREEN_H__
#define __WIESEL_VIDEO_SCREEN_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/geometry.h>
#include <wiesel/math/matrix.h>
#include <wiesel/device.h>

namespace wiesel {

	class Platform;
	class TouchHandler;


namespace video {

	class IVideoLoader;
	class VideoDeviceDriver;



	enum VideoState {
		/**
		 * @brief The current screen has no acitive \ref VideoDevice, so no rendering is possible.
		 */
		Video_Uninitialized,

		/**
		 * @brief The attached video device is avtive and ready for rendering.
		 */
		Video_Active,

		/**
		 * @brief The attached video device is ready for rendering, but the output device is not (fully) visible.
		 * On PC platforms, the window may be hidden bihind other windows. On smartphones or tables, there may be a notification
		 * or popup in the foreground.
		 * Rendering is still possible, but the application may use a lower framerate.
		 */
		Video_Background,

		/**
		 * @brief The video device is still present, but fully hidden.
		 * This would happen, when the application is not visible (for example: the display was switched off or other applications
		 * are in the user focus)
		 * The application should sleep in this state and don't perform any rendering.
		 */
		Video_Suspended,
	};


	/**
	 * @brief A set of flags for configuring the \ref Screen's \ref VideoDevice
	 * and \ref RenderContext.
	 */
	enum VideoFlags {
		/**
		 * @brief Tries to create a fullscreen \ref VideoDefice.
		 * Fails, if fullscreen is not possible.
		 * If this flag is omitted, the loader tries to create a \ref VideoDevice
		 * in window mode, but some platforms like android or iOS may just create
		 * fullscreen devices.
		 */
		Video_Fullscreen			= 0x0001,
	};


	/**
	 * @brief An abstract interface to the current screen.
	 */
	class WIESEL_CORE_EXPORT Screen : public Device
	{
	public:
		Screen();
		virtual ~Screen();

		/// get the screen's current state
		VideoState getState() const;

		/// get this screen's touchhandler
		inline TouchHandler *getTouchHandler() {
			return touch_handler;
		}

		/// get the currently assigned video device
		inline VideoDeviceDriver *getVideoDeviceDriver() {
			return video_device_driver;
		}

		/// get the currently assigned vicode device
		inline const VideoDeviceDriver *getVideoDeviceDriver() const {
			return video_device_driver;
		}

	public:
		/**
		 * Tries to load a new \ref VideoDevice into this screen object.
		 * @param resolution	The resolution of the new \ref VideoDevice.
		 * @param flags			Flags to determine the properties of the new \ref VideoDevice.
		 * @return				\c true on success.
		 */
		bool loadVideoDevice(const dimension &resolution, unsigned int flags);
		/**
		 * @brief Change the current driver of this video device.
		 * Switching the driver will force all resources to be reloaded.
		 */
		void setVideoDeviceDriver(VideoDeviceDriver *driver);

	protected:
		TouchHandler*		touch_handler;
		VideoDeviceDriver*	video_device_driver;
	};

}
}

#endif /* __WIESEL_VIDEO_SCREEN_H__ */
