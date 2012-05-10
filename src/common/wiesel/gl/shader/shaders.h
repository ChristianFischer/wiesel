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
#ifndef __WIESEL_GL_SHADER_SHADERS_H__
#define	__WIESEL_GL_SHADER_SHADERS_H__

#include "shader.h"

#include "wiesel/gl/vbo/vertexbuffer.h"
#include "wiesel/util/shared_object_cache.h"

#include <string>


namespace wiesel {

	/**
	 * @brief An utility class to create new shaders
	 * and cache existing shaders.
	 */
	class Shaders
	{
	private:
		Shaders();
		~Shaders();

	// singleton
	public:
		/// get the singleton instance
		static Shaders *instance();

	// shader variable names
	public:
		static const char *ATTRIBUTE_VERTEX_POSITION;
		static const char *ATTRIBUTE_VERTEX_NORMAL;
		static const char *ATTRIBUTE_VERTEX_COLOR;
		static const char *ATTRIBUTE_VERTEX_TEXTURE_COORDINATE;
		static const char *UNIFORM_PROJECTION_MATRIX;
		static const char *UNIFORM_MODELVIEW_MATRIX;
		static const char *UNIFORM_TEXTURE;
		static const char *VARYING_COLOR;
		static const char *VARYING_NORMAL;
		static const char *VARYING_TEXTURE_COORDINATE;

	// public types
	public:
		/// alias type for a cache for shader objects
		typedef SharedObjectCache<std::string,Shader>			ShaderCache;

		/// alias type for a cache for shader program objects
		typedef SharedObjectCache<std::string,ShaderProgram>	ShaderProgramCache;

	// caching
	public:
		/// get the cache for vertex shaders
		inline ShaderCache *getVertexShaderCache() {
			return &cached_vertex_shaders;
		}

		/// get the cache for fragment shaders
		inline ShaderCache *getFragmentShaderCache() {
			return &cached_fragment_shaders;
		}

		/// get the cache for shader programs
		inline ShaderProgramCache *getShaderProgramCache() {
			return &cached_shader_programs;
		}

	// vertex buffer shaders
	public:
		/// get a suitable shader for a given \ref VertexBuffer.
		ShaderProgram *getShaderFor(VertexBuffer *vbo);

		/// get a suitable vertex shader for a given \ref VertexBuffer.
		Shader *getVertexShaderFor(VertexBuffer *vbo);

		/// get a suitable fragment shader for a given \ref VertexBuffer.
		Shader *getFragmentShaderFor(VertexBuffer *vbo);

	private:
		ShaderCache				cached_vertex_shaders;
		ShaderCache				cached_fragment_shaders;
		ShaderProgramCache		cached_shader_programs;
	};

}

#endif	/* __WIESEL_GL_SHADER_SHADERS_H__ */

