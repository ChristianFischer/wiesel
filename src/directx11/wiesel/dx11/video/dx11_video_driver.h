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
#ifndef __WIESEL_DX11_VIDEO_DX11_VIDEO_DRIVER_H__
#define __WIESEL_DX11_VIDEO_DX11_VIDEO_DRIVER_H__

#include <wiesel/wiesel-directx11.def>

#include "dx11_render_context.h"
#include "dx11_shader_content.h"
#include "dx11_vertexbuffer_content.h"

#include <wiesel/video/screen.h>
#include <wiesel/video/video_driver.h>
#include <wiesel/engine_interfaces.h>

#include "wiesel/dx11/dx11.h"


namespace wiesel {
namespace dx11 {
namespace video {

	/**
	 * @brief The SDL video device implementation.
	 */
	class WIESEL_DIRECTX11_EXPORT Dx11VideoDeviceDriver :
			public wiesel::video::VideoDeviceDriver,
			protected wiesel::IUpdateable
	{
	public:
		Dx11VideoDeviceDriver(wiesel::video::Screen *screen);
		virtual ~Dx11VideoDeviceDriver();

	public:
		bool init(const dimension &size, unsigned int flags);
		bool shutdown();

	protected:
		bool initWindow(const dimension &size, int nCmdShow);
		bool initDirectX();

		virtual void updateScreenSize(float w, float h);

	// IUpdateable
	protected:
		virtual void update(float dt);

	public:
		virtual vector2d convertScreenToWorld(const vector2d &screen) const;

		virtual void preRender();
		virtual void postRender();

		virtual wiesel::video::RenderContext *getCurrentRenderContext();

	// resource management
	public:
		virtual wiesel::video::IndexBufferContent *createIndexBufferContent(wiesel::video::IndexBuffer *index_buffer);
		virtual wiesel::video::VertexBufferContent *createVertexBufferContent(wiesel::video::VertexBuffer *index_buffer);
		virtual wiesel::video::ShaderContent *createShaderContent(wiesel::video::Shader *shader);
		virtual wiesel::video::ShaderConstantBufferContent *createConstantBufferContent(wiesel::video::ShaderConstantBuffer *shader_constant_buffer);
		virtual wiesel::video::TextureContent *createTextureContent(wiesel::video::Texture *texture);
		virtual wiesel::video::RenderBufferContent *createRenderBufferContent(wiesel::video::RenderBuffer *render_buffer);

	private:
		HINSTANCE					hInstance;
		HWND						hWnd;

		DirectX11RenderContext*		render_context;
	};

}
}
}

#endif // __WIESEL_DX11_VIDEO_DX11_VIDEO_DRIVER_H__
