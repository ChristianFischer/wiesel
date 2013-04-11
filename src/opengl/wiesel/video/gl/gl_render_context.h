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
#ifndef __WIESEL_VIDEO_GL_RENDER_CONTEXT_H__
#define	__WIESEL_VIDEO_GL_RENDER_CONTEXT_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/video/render_context.h>
#include <wiesel/video/indexbuffer.h>
#include <wiesel/video/shader.h>
#include <wiesel/video/texture.h>
#include <wiesel/video/vertexbuffer.h>

#include "gl_shader_content.h"
#include "gl_shader_constantbuffer_content.h"
#include "gl_texture_content.h"


namespace wiesel {
namespace video {
namespace gl {


	class WIESEL_OPENGL_EXPORT OpenGlRenderContext : public RenderContext
	{
	public:
		OpenGlRenderContext(Screen *screen);
		virtual ~OpenGlRenderContext();

	public:
		virtual void initContext();
		virtual void releaseContext();

	public:
		virtual void onSizeChanged(const dimension &size);

	public:
		virtual void preRender();
		virtual void postRender();

	public:
		virtual void setProjectionMatrix(const matrix4x4 &matrix);
		virtual void setModelviewMatrix(const matrix4x4 &matrix);

	public:
		virtual void setShader(Shader *shader);
		virtual bool assignShaderConstantBuffer(const std::string &name, ShaderConstantBuffer *buffer);

		virtual void setTexture(uint16_t index, Texture *texture);
		virtual void prepareTextureLayers(uint16_t layers);
		virtual void clearTextures();

		virtual void draw(Primitive primitive, const VertexBuffer *vertices);
		virtual void draw(Primitive primitive, const VertexBuffer *vertices, const IndexBuffer *indices);

	protected:
		bool bind(const IndexBuffer *index_buffer);
		bool bind(const VertexBuffer *vertex_buffer);

		void unbind(const IndexBuffer *index_buffer);
		void unbind(const VertexBuffer *vertex_buffer);

	private:
		Shader*								active_shader;
		GlShaderContent*					active_shader_content;
		std::vector<Texture*>				active_textures;
		std::vector<GlTextureContent*>		active_textures_content;

		GlShaderConstantBufferContent*		cb_modelview_content;
		GlShaderConstantBufferContent*		cb_projection_content;
	};

}
}
}

#endif	// __WIESEL_VIDEO_GL_RENDER_CONTEXT_H__
