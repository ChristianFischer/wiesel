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
#ifndef __WIESEL_VIDEO_GL_SHADER_CONTENT_H__
#define __WIESEL_VIDEO_GL_SHADER_CONTENT_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/video/shader.h>

#include "gl.h"


namespace wiesel {
namespace video {
namespace gl {

	class OpenGlRenderContext;

	/**
	 * @brief OpenGL backend for shader objects.
	 */
	class WIESEL_OPENGL_EXPORT GlShaderContent : public ShaderContent
	{
	private:
		GlShaderContent(Shader *shader);

	public:
		virtual ~GlShaderContent();

		/**
		 * @brief Crerates an OpenGL shader content for the given shader.
		 * @param shader	The shader object where to load the content object from.
		 * @return A content object on success, \c NULL when failed.
		 */
		static WIESEL_OPENGL_EXPORT GlShaderContent *createContentFor(Shader *shader);

	public:
		/**
		 * @brief Initialize the shader program.
		 */
		void initializeShader();

		/**
		 * @brief Releases the shader.
		 */
		void releaseShader();

		/**
		 * @brief Tries to bind all configured attributes of the attached shader.
		 */
		void bindAttributes();

		/**
		 * @brief Get the OpenGL handle of the current shader program.
		 */
		inline GLuint getGlHandle() const {
			return program_handle;
		}

	public:
		/**
		 * @brief Get the OpenGL handle of a specific shader attribute.
		 * @param attr		The attributes type.
		 * @param index		The index of multiple attributes (like the index
		 *					of the texture unit on multitextured vertices)
		 * @return			The OpenGL handle.
		 *					It depends on the attribute type, if the handle
		 *					refers to an attribute or uniform parameter.
		 */
		GLint getAttribHandle(Shader::Attribute attr, uint8_t index) const;

		/// assigns a constant buffer to the current shader instance.
		virtual bool assignShaderConstantBuffer(const std::string &name, ShaderConstantBufferContent *buffer_content);

		/// set a uniform shader value
		virtual bool setShaderValue(const std::string &name, ValueType type, size_t elements, void *pValue);

	private:
		/// Alias type for an indiced list of attribute names
		typedef std::vector<GLint>						AttributeHandlesByIndex;

		/// Alias type for a list of attributes
		typedef std::vector<AttributeHandlesByIndex>	AttributeHandleList;

		typedef std::map<
					const ShaderConstantBufferTemplate*,
					ShaderConstantBuffer::version_t
		>												BufferVersionMap;

		GLuint		program_handle;

		AttributeHandleList			attribute_handles;
		BufferVersionMap			buffer_versions;

		std::map<std::string,GLint>	uniform_attributes;
	};

}
}
}

#endif /* __WIESEL_VIDEO_GL_SHADER_CONTENT_H__ */
