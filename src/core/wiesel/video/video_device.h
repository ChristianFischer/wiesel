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
#ifndef __WIESEL_VIDEO_VIDEODEVICE_H__
#define __WIESEL_VIDEO_VIDEODEVICE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/geometry.h>
#include <wiesel/math/matrix.h>
#include <wiesel/device.h>

#include "screen.h"

namespace wiesel {
namespace video {

	/**
	 * @brief An abstract interface to a video output device.
	 */
	class WIESEL_CORE_EXPORT VideoDevice : public Device
	{
	private:
		VideoDevice();

	public:
		VideoDevice(Screen *screen);
		virtual ~VideoDevice();

		/// get the device's current state
		inline VideoState getState() const {
			return state;
		}

		/// get the current resolution in pixels
		inline const dimension& getResolution() const {
			return size;
		}

		/// get the screen, this video device is currently assigned to
		inline Screen *getScreen() {
			return screen;
		}

		/// get the current aspect ratio of the screen
		inline float getAspectRatio() const {
			return size.width / size.height;
		}

		/// get the renderer's projection matrix
		inline const matrix4x4& getProjectionMatrix() const {
			return projection;
		}

	// overridables
	public:
		/// called before rendering a frame
		virtual void preRender() = 0;

		/// called after rendering a frame
		virtual void postRender() = 0;

	// member functions
	protected:
		/// set the device's current video state
		void setState(VideoState state);

		/// update the device's screen size
		virtual void updateScreenSize(float w, float h);

	protected:
		VideoState	state;
		Screen*		screen;
		matrix4x4	projection;
		dimension	size;
	};

}
}

#endif /* __WIESEL_VIDEO_VIDEODEVICE_H__ */
