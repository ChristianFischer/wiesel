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
#ifndef __WIESEL_SCREEN_H__
#define __WIESEL_SCREEN_H__

#include <wiesel/wiesel-common.def>

#include <wiesel/geometry.h>
#include <wiesel/math/matrix.h>

namespace wiesel {

	/**
	 * @brief An abstract interface to the current screen.
	 */
	class WIESEL_COMMON_EXPORT Screen {
	public:
		Screen();
		virtual ~Screen();

		/// get the current screen size in pixels
		inline const dimension& getSize() const {
			return size;
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
		virtual void updateScreenSize(float w, float h);

	protected:
		matrix4x4	projection;
		dimension	size;
	};

}

#endif /* __WIESEL_SCREEN_H__ */
