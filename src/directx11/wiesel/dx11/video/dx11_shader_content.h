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
#ifndef __WIESEL_DX11_VIDEO_SHADER_CONTENT_H__
#define __WIESEL_DX11_VIDEO_SHADER_CONTENT_H__

#include <wiesel/wiesel-directx11.def>

#include <wiesel/video/shader.h>
#include <wiesel/video/vertexbuffer.h>

#include "wiesel/dx11/dx11.h"

namespace wiesel {
namespace dx11 {
namespace video {

	class DirectX11RenderContext;
	class Dx11ShaderConstantBufferContent;
	class Dx11TextureContent;

	/**
	 * @brief DirectX11 backend for shader objects.
	 */
	class WIESEL_DIRECTX11_EXPORT Dx11ShaderContent : public wiesel::video::ShaderContent
	{
	private:
		Dx11ShaderContent(DirectX11RenderContext *context, wiesel::video::Shader *shader);

	public:
		virtual ~Dx11ShaderContent();

		/**
		 * @brief Crerates an OpenGL shader content for the given shader.
		 * @param shader	The shader object where to load the content object from.
		 * @return A content object on success, \c NULL when failed.
		 */
		static Dx11ShaderContent *createContentFor(DirectX11RenderContext *context, wiesel::video::Shader *shader);

	public:
		/**
		 * @brief Initialize the shader program.
		 */
		bool initializeShader(DirectX11RenderContext *context);

		/**
		 * @brief Releases the shader.
		 */
		void releaseShader();

		/**
		 * @brief Tries to bind all configured attributes of the attached shader.
		 */
		bool bindAttributes(DirectX11RenderContext *context);

	public:
		/// assigns a constant buffer to the current shader instance.
		virtual bool assignShaderConstantBuffer(const std::string &name, wiesel::video::ShaderConstantBufferContent *buffer_content);

		/// assigns a constant buffer to the current shader instance.
		bool assignShaderConstantBuffer(const std::string &name, Dx11ShaderConstantBufferContent *buffer_content);

		/// bind this shader to the context
		bool bind(DirectX11RenderContext *render_context);

		/// bind the given vertex_buffer to this shader
		bool bind(DirectX11RenderContext *render_context, const wiesel::video::VertexBuffer *vertex_buffer, const std::vector<Dx11TextureContent*> &textures);

	private:
		struct ShaderConstantBufferEntry {
			uint32_t	vs_slot;
			uint32_t	ps_slot;
		};

		typedef std::map<std::string, ID3D10Blob*>			ShaderOpcodeBufferMap;
		typedef std::map<std::string, ID3D11InputLayout*>	PolygonLayoutMap;

		typedef std::map<
					const wiesel::video::ShaderConstantBufferTemplate*,
					ShaderConstantBufferEntry
		>												ShaderConstantBufferEntryMap;

		ShaderOpcodeBufferMap			shader_opcode_buffers;
		ShaderConstantBufferEntryMap	shader_constant_buffer_entries;
		ID3D11VertexShader*				vertex_shader;
		ID3D11PixelShader*				pixel_shader;
		PolygonLayoutMap				polygon_layouts;

		DirectX11RenderContext*			render_context;
	};

}
}
}

#endif /* __WIESEL_DX11_VIDEO_SHADER_CONTENT_H__ */
