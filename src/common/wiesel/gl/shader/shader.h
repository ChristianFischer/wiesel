/*
 * Shader.h
 *
 *  Created on: 28.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_GL_SHADER_SHADER_H__
#define __WIESEL_GL_SHADER_SHADER_H__

#include <wiesel/gl/gl.h>
#include <wiesel/util/managed_object.h>
#include <string>
#include <vector>


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
	class Shader : public ManagedObject
	{
	private:
		Shader();

	public:
		virtual ~Shader();

		/**
		 * @brief compile a shader from source code.
		 */
		static Shader *compile(const std::string &source, ShaderType type);

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
		/// name of the vertex position attribute
		std::string		attrib_vertex_position;
		std::string		attrib_vertex_normal;
		std::string		attrib_vertex_color;

		/// names of texture coordinate attributes
		std::vector<std::string>	attrib_vertex_texcoords;

	// attributes (fragment shader)
	public:
		/// names of texture coordinate attributes
		std::vector<std::string>	attrib_vertex_textures;

	private:
		/**
		 * @brief release the shader.
		 * Needs to be used before deleting this object.
		 */
		void release_shader();

	private:
		ShaderType		type;
		GLuint			shader;
	};



	/**
	 * @brief Handles a shader program with mulitple shaders linked.
	 */
	class ShaderProgram : public ManagedObject
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
		 * @brief bind all attributes configured in the shaders.
		 */
		void bindAttributes();

		/**
		 * @brief binds this program to the current OpenGL context.
		 */
		void bind() const;

		/**
		 * @brief removes this program from the current OpenGL context.
		 */
		void unbind() const;

	// attribute access
	public:
		inline GLuint getVertexPositionAttribute() const {
			return attrib_handle_vertex_position;
		}

		inline GLuint getVertexNormalAttribute() const {
			return attrib_handle_vertex_normal;
		}

		inline GLuint getVertexColorAttribute() const {
			return attrib_handle_vertex_color;
		}

		inline GLuint getVertexTextureCoordAttribute(unsigned int layer) const {
			return (attrib_handle_vertex_texcoords.size() > layer) ? attrib_handle_vertex_texcoords.at(layer) : 0;
		}

		inline GLuint getVertexTextureAttribute(unsigned int layer) const {
			return (attrib_handle_vertex_textures.size() > layer) ? attrib_handle_vertex_textures.at(layer) : 0;
		}

	private:
		/**
		 * @brief release the shader.
		 */
		void release_shader();

	// private members
	private:
		std::vector<Shader*>	shaders;
		GLuint					program;
		bool					need_link;

		GLuint					attrib_handle_vertex_position;
		GLuint					attrib_handle_vertex_normal;
		GLuint					attrib_handle_vertex_color;
		std::vector<GLuint>		attrib_handle_vertex_texcoords;
		std::vector<GLuint>		attrib_handle_vertex_textures;
	};

}

#endif /* __WIESEL_GL_SHADER_SHADER_H__ */
