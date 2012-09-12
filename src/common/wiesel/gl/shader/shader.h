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
#ifndef __WIESEL_GL_SHADER_SHADER_H__
#define __WIESEL_GL_SHADER_SHADER_H__

#include <wiesel/gl/gl.h>
#include <wiesel/math/matrix.h>
#include <wiesel/io/datasource.h>
#include <wiesel/util/shared_object.h>
#include <string>
#include <vector>
#include <map>


namespace wiesel {


	/**
	 * @brief Handles for each available shader type.
	 */
	enum ShaderType
	{
		ShaderType_VertexShader,		//!< Vertex Shader applies transformations to each vertex.
		ShaderType_FragmentShader,		//!< Fragment Shaders will be applied to each pixel.
	};




	/**
	 * @brief A class handling a single shader object.
	 */
	class WIESEL_COMMON_EXPORT Shader : public virtual SharedObject
	{
	private:
		Shader();

	public:
		/**
		 * @brief Create a new shader with source code from a string
		 * The created shader is not compiled. Call compile() to compile
		 * the shader or use compile(const std::string&, ShaderType)
		 * to get a compiled shader.
		 */
		Shader(const std::string &source, ShaderType type);

		/**
		 * @brief Create a new shader with source code from a DataSource
		 * The created shader is not compiled. Call compile() to compile
		 * the shader or use compile(const std::string&, ShaderType)
		 * to get a compiled shader.
		 */
		Shader(DataSource *source, ShaderType type);

		virtual ~Shader();

		/**
		 * @brief compile a shader from source code.
		 * @return the created shader on success, \c NULL when compiling failed.
		 */
		static WIESEL_COMMON_EXPORT Shader *compile(const std::string &source, ShaderType type);

		/**
		 * @brief compile a shader from source code.
		 * @return the created shader on success, \c NULL when compiling failed.
		 */
		static WIESEL_COMMON_EXPORT Shader *compile(DataSource *source, ShaderType type);

		/**
		 * 
		 * @return \c true when the shader was compiled successfully, \c false otherwise.
		 */
		bool compile();

		/**
		 * @brief Add the name of a new uniform attribute.
		 */
		void addUniform(const std::string &name);

		/**
		 * @brief get the type of this shader.
		 */
		inline ShaderType getType() const {
			return type;
		}

		/**
		 * @brief get access to the OpenGL shader handle.
		 */
		inline GLuint getGlShader() {
			return shader;
		}

	// attributes (vertex shader)
	public:
		std::string		uniform_projection_matrix;
		std::string		uniform_modelview_matrix;
		std::string		attrib_vertex_position;
		std::string		attrib_vertex_normal;
		std::string		attrib_vertex_color;

		/// names of texture coordinate attributes
		std::vector<std::string>	attrib_vertex_texcoords;

	// attributes (fragment shader)
	public:
		/// names of texture coordinate attributes
		std::vector<std::string>	attrib_vertex_textures;

	// attributes (custom)
	public:
		/// names of custom uniform attributes to be set from user code
		std::vector<std::string>	uniform_attributes;

	private:
		/**
		 * @brief release the shader.
		 * Needs to be used before deleting this object.
		 */
		void release_shader();

	private:
		DataSource*		source;
		ShaderType		type;
		GLuint			shader;
	};



	/**
	 * @brief Handles a shader program with mulitple shaders linked.
	 */
	class WIESEL_COMMON_EXPORT ShaderProgram : public virtual SharedObject
	{
	public:
		/**
		 * @brief Create a new \ref ShaderProgram with no shaders attached.
		 */
		ShaderProgram();

		virtual ~ShaderProgram();


	public:
		/**
		 * @brief Attaches a new shader to this program.
		 */
		bool attach(Shader *shader);

		/**
		 * @brief link the shader program after attaching all shaders.
		 */
		bool link();

		/**
		 * @brief binds this program to the current OpenGL context.
		 */
		void bind() const;

		/**
		 * @brief removes this program from the current OpenGL context.
		 */
		void unbind() const;

		/**
		 * @brief check, if this shader is currently bound to the GL context.
		 */
		bool isBound() const;

	// attribute access
	public:
		inline GLint getVertexPositionAttribute() const {
			return attrib_handle_vertex_position;
		}

		inline GLint getVertexNormalAttribute() const {
			return attrib_handle_vertex_normal;
		}

		inline GLint getVertexColorAttribute() const {
			return attrib_handle_vertex_color;
		}

		inline GLint getVertexTextureCoordAttribute(unsigned int layer) const {
			return (attrib_handle_vertex_texcoords.size() > layer) ? attrib_handle_vertex_texcoords.at(layer) : -1;
		}

		inline GLint getVertexTextureAttribute(unsigned int layer) const {
			return (attrib_handle_vertex_textures.size() > layer) ? attrib_handle_vertex_textures.at(layer) : -1;
		}

		inline GLint getProjectionMatrixHandle() const {
			return uniform_handle_projection_matrix;
		}

		inline GLint getModelviewMatrixHandle() const {
			return uniform_handle_modelview_matrix;
		}

	public:
		/// set the default values of some uniform parameters like modelview- or projection-matrix
		void setDefaultValues();

		/// set the value of an uniform attribute
		void set(const std::string &name, int value);

		/// set the value of an uniform attribute
		void set(const std::string &name, float value);

		/// set the projection matrix
		void setProjectionMatrix(const matrix4x4 &matrix);

		/// set the modelview matrix
		void setModelviewMatrix(const matrix4x4 &matrix);

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
	};

}

#endif /* __WIESEL_GL_SHADER_SHADER_H__ */
