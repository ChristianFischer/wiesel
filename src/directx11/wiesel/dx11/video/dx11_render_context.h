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
#ifndef __WIESEL_DX11_VIDEO_DX11_RENDER_CONTEXT_H__
#define	__WIESEL_DX11_VIDEO_DX11_RENDER_CONTEXT_H__

#include <wiesel/wiesel-directx11.def>

#include <wiesel/video/render_context.h>
#include <wiesel/video/indexbuffer.h>
#include <wiesel/video/shader.h>
#include <wiesel/video/texture.h>
#include <wiesel/video/vertexbuffer.h>
#include <wiesel/video/video_driver.h>

#include "wiesel/dx11/dx11.h"
#include "dx11_shader_content.h"
#include "dx11_texture_content.h"


namespace wiesel {
namespace dx11 {
namespace video {


	class WIESEL_DIRECTX11_EXPORT DirectX11RenderContext : public wiesel::video::RenderContext
	{
	private:
		DirectX11RenderContext(wiesel::video::Screen *screen);

	public:
		virtual ~DirectX11RenderContext();

	public:
		static DirectX11RenderContext *createContextWithWindowHandle(HWND hWnd, wiesel::video::Screen *screen, wiesel::video::VideoInfo *pVideoInfo);

		virtual void initContext();
		virtual void releaseContext();

	public:
		/// get the Direct3D device.
		inline ID3D11Device *getD3DDevice() const {
			return d3d_device;
		}

		/// get the Direct3D device context.
		inline ID3D11DeviceContext *getD3DDeviceContext() {
			return d3d_device_context;
		}

	public:
		virtual void onSizeChanged(const dimension &size);

	public:
		virtual void preRender();
		virtual void postRender();

	public:
		virtual void setProjectionMatrix(const matrix4x4 &matrix);
		virtual void setModelviewMatrix(const matrix4x4 &matrix);

	public:
		virtual void setShader(wiesel::video::Shader *shader);
		virtual void setShaderValue(const std::string &name, wiesel::video::Shader::ValueType type, size_t elements, void *pValue);
		virtual void setTexture(uint16_t index, wiesel::video::Texture *texture);
		virtual void prepareTextureLayers(uint16_t layers);
		virtual void clearTextures();

		virtual void draw(wiesel::video::Primitive primitive, const wiesel::video::VertexBuffer *vertices);
		virtual void draw(wiesel::video::Primitive primitive, const wiesel::video::VertexBuffer *vertices, const wiesel::video::IndexBuffer *indices);

	protected:
		bool bind(const wiesel::video::IndexBuffer *index_buffer);
		bool bind(const wiesel::video::VertexBuffer *vertex_buffer);

		void unbind(const wiesel::video::IndexBuffer *index_buffer);
		void unbind(const wiesel::video::VertexBuffer *vertex_buffer);

	private:
		bool									vsync;
		bool									fullscreen;

		D3D_DRIVER_TYPE							driver_type;
		D3D_FEATURE_LEVEL						feature_level;
		ID3D11Device*							d3d_device;
		ID3D11DeviceContext*					d3d_device_context;
		IDXGISwapChain*							swap_chain;
		ID3D11RenderTargetView*					render_target_view;

		ID3D11Texture2D*						depth_stencil_buffer;
		ID3D11DepthStencilState*				depth_stencil_state;
		ID3D11DepthStencilView*					depth_stencil_view;
		ID3D11RasterizerState*					raster_state;

		ID3D11BlendState*						blendstate_enabled;

		wiesel::video::Shader*					active_shader;
		Dx11ShaderContent*						active_shader_content;
		std::vector<wiesel::video::Texture*>	active_textures;
		std::vector<Dx11TextureContent*		>	active_textures_content;
	};

}
}
}

#endif	// __WIESEL_DX11_VIDEO_DX11_RENDER_CONTEXT_H__
