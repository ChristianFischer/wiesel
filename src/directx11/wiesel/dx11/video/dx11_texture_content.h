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
#ifndef __WIESEL_DX11_VIDEO_TEXTURE_CONTENT_H__
#define __WIESEL_DX11_VIDEO_TEXTURE_CONTENT_H__

#include <wiesel/wiesel-directx11.def>

#include <wiesel/geometry.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/video/texture.h>
#include <string>

#include "wiesel/dx11/dx11.h"


namespace wiesel {
namespace dx11 {
namespace video {

	class DirectX11RenderContext;

	/**
	 * @brief DirectX11 Backend for texture objects.
	 * @return
	 */
	class WIESEL_DIRECTX11_EXPORT Dx11TextureContent : public wiesel::video::TextureContent
	{
	private:
		Dx11TextureContent(wiesel::video::Texture *texture);

	public:
		virtual ~Dx11TextureContent();

		/**
		 * @brief Crerates an DirectX11 texture content for the given texture.
		 * @param texture	The texture object where to load the content object from.
		 * @return A content object on success, \c NULL when failed.
		 */
		static Dx11TextureContent *createContentFor(DirectX11RenderContext *context, wiesel::video::Texture *texture);

	private:
		bool initializeTexture(DirectX11RenderContext *context);

		/**
		 * @brief Releases the index buffer object on the graphics hardware.
		 */
		void releaseTexture();

	public:
		/**
		 * @brief Get the DirectX11 Texture resource.
		 */
		inline ID3D11Texture2D* getDx11Texture() const {
			return texture;
		}

		/**
		 * @brief Get the DirectX11 Texture shader resource.
		 */
		inline ID3D11ShaderResourceView* getDx11ShaderResourceView() const {
			return shader_resource_view;
		}

		/**
		 * @brief Get the DirectX11 Texture sampler state.
		 */
		inline ID3D11SamplerState* getDx11TextureSamplerState() const {
			return sampler_state;
		}

	private:
		ID3D11Texture2D*			texture;
		ID3D11ShaderResourceView*	shader_resource_view;
		ID3D11SamplerState*			sampler_state;
	};

} /* namespace video */
} /* namespace dx11 */
} /* namespace wiesel */
#endif /* __WIESEL_DX11_VIDEO_TEXTURE_CONTENT_H__ */
