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
#ifndef __WIESEL_VIDEO_VIDEOLOADER_H__
#define __WIESEL_VIDEO_VIDEOLOADER_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/module.h>

#include "screen.h"


namespace wiesel {
namespace video {

	class Screen;
	class VideoDeviceDriver;

	/**
	 * @brief The SDL video device implementation.
	 */
	class WIESEL_CORE_EXPORT IVideoLoader : public Module
	{
	public:
		IVideoLoader();
		virtual ~IVideoLoader();

	public:
		/**
		 * @brief Tries to initialize the given screen object with a new \ref VideoDeviceDriver.
		 * @param screen		The screen object to be initialilize.
		 * @param resolution	The requiested resolution of this device.
		 * @param flags			A set of flags for the new video device.
		 * @return				\c true on success, \c false otherwise.
		 */
		virtual bool loadVideoDevice(wiesel::video::Screen *screen, const dimension &resolution, unsigned int flags) = 0;

	protected:
		/**
		 * @brief Tries to install a created \ref VideoDeviceDriver into the given \ref Screen object.
		 * @param screen		The screen, the new video device will be installed to
		 * @param driver		The video device, which will installed.
		 * @return				\c true on success, \c false otherwise.
		 */
		virtual bool install(Screen *screen, VideoDeviceDriver *driver);
	};

}
}

#endif // __WIESEL_SDL_VIDEO_SDLVIDEOLOADER_H__
