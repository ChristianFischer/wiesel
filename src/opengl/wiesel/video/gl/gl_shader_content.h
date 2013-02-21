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

		/// set the projection matrix
		virtual bool setProjectionMatrix(const matrix4x4 &matrix);

		/// set the modelview matrix
		virtual bool setModelviewMatrix(const matrix4x4 &matrix);

#if 0
	private:
		/**
		 * @brief bind all attributes configured in the shaders.
		 */
		void bindAttributes();

		/**
		 * @brief get the OpenGL handle to a specific uniform parameter.
		 */
		GLint getUniformHandle(const std::string &name) const;

		/**
		 * @brief release the shader.
		 */
		void release_shader();

	// private members
	private:
		std::vector<Shader*>	shaders;
		GLuint					program;
		bool					need_link;

		GLint					uniform_handle_projection_matrix;
		GLint					uniform_handle_modelview_matrix;
		GLint					attrib_handle_vertex_position;
		GLint					attrib_handle_vertex_normal;
		GLint					attrib_handle_vertex_color;
		std::vector<GLint>		attrib_handle_vertex_texcoords;
		std::vector<GLint>		attrib_handle_vertex_textures;

		std::map<std::string,GLint>	uniform_attributes;
#endif
	private:
		/// Alias type for an indiced list of attribute names
		typedef std::vector<GLint>						AttributeHandlesByIndex;

		/// Alias type for a list of attributes
		typedef std::vector<AttributeHandlesByIndex>	AttributeHandleList;


		GLuint		program_handle;

		GLint		modelview_matrix_handle;
		GLint		projection_matrix_handle;

		AttributeHandleList			attribute_handles;

		std::map<std::string,GLint>	uniform_attributes;
	};

}
}
}

#endif /* __WIESEL_VIDEO_GL_SHADER_CONTENT_H__ */
