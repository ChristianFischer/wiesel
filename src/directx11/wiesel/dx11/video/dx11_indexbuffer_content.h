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
#ifndef __WIESEL_DX11_VIDEO_INDEXBUFFER_CONTENT_H__
#define __WIESEL_DX11_VIDEO_INDEXBUFFER_CONTENT_H__

#include <wiesel/wiesel-directx11.def>

#include <wiesel/video/indexbuffer.h>
#include <wiesel/dx11/dx11.h>

namespace wiesel {
namespace dx11 {
namespace video {

	class DirectX11RenderContext;


	/**
	 * @brief OpenGL backend for indexbuffer objects.
	 */
	class WIESEL_DIRECTX11_EXPORT Dx11IndexBufferContent : public wiesel::video::IndexBufferContent
	{
	private:
		Dx11IndexBufferContent(wiesel::video::IndexBuffer *ib);

	public:
		virtual ~Dx11IndexBufferContent();

		static Dx11IndexBufferContent *createContentFor(DirectX11RenderContext *context, wiesel::video::IndexBuffer *ib);

	public:
		/**
		 * @brief Initialize the index buffer data on the graphics hardware.
		 */
		bool initializeIndexBuffer(DirectX11RenderContext *context);

		/**
		 * @brief Releases the index buffer object on the graphics hardware.
		 */
		void releaseIndexBuffer();

		/**
		 * @brief Get the DirectX11 buffer object.
		 */
		inline ID3D11Buffer *getDx11Buffer() const {
			return buffer;
		}

	private:
		ID3D11Buffer*	buffer;
	};

} /* namespace video */
} /* namespace dx11 */
} /* namespace wiesel */
#endif /* __WIESEL_DX11_VIDEO_INDEXBUFFER_CONTENT_H__ */
