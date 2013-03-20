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
#ifndef __WIESEL_DX11_VIDEO_DX11_VIDEO_LOADER_H__
#define __WIESEL_DX11_VIDEO_DX11_VIDEO_LOADER_H__

#include <wiesel/wiesel-directx11.def>

#include <wiesel/video/screen.h>
#include <wiesel/video/video_loader.h>


namespace wiesel {
namespace dx11 {
namespace video {

	/**
	 * @brief The SDL video device implementation.
	 */
	class WIESEL_DIRECTX11_EXPORT Dx11VideoLoader :
			public wiesel::video::IVideoLoader
	{
	public:
		Dx11VideoLoader();
		virtual ~Dx11VideoLoader();

	public:
		virtual bool loadVideoDevice(wiesel::video::Screen *screen, const dimension &resolution, unsigned int flags);
	};

}
}
}

#endif // __WIESEL_DX11_VIDEO_DX11_VIDEO_LOADER_H__
