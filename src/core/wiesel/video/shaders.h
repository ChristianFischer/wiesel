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

#include <wiesel/wiesel-core.def>

#include "wiesel/util/shared_object_cache.h"
#include "wiesel/video/shader.h"
#include "wiesel/video/vertexbuffer.h"

#include <string>


namespace wiesel {
namespace video {

	/**
	 * @brief An utility class to create new shaders
	 * and cache existing shaders.
	 */
	class WIESEL_CORE_EXPORT Shaders
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
		/// alias type for a cache for data sources
		typedef SharedObjectCache<std::string,DataSource>		DataSourceCache;

		/// alias type for a cache for shader objects
		typedef SharedObjectCache<std::string,Shader>			ShaderCache;

	// caching
	public:
		/// get the cache for vertex shader data sources
		inline DataSourceCache *getGlslVertexShaderCache() {
			return &cached_glsl_vertex_shaders;
		}

		/// get the cache for fragment shader data sources
		inline DataSourceCache *getGlslFragmentShaderCache() {
			return &cached_glsl_fragment_shaders;
		}

		/// get the cache for vertex shader data sources
		inline DataSourceCache *getHlslVertexShaderCache() {
			return &cached_hlsl_vertex_shaders;
		}

		/// get the cache for fragment shader data sources
		inline DataSourceCache *getHlslFragmentShaderCache() {
			return &cached_hlsl_fragment_shaders;
		}

		/// get the cache for shaders
		inline ShaderCache *getShaderCache() {
			return &cached_shaders;
		}

	// vertex buffer shaders
	public:
		/// get a suitable shader for a given \ref VertexBuffer.
		Shader *getShaderFor(VertexBuffer *vbo);

		/// get a suitable vertex shader source for a given \ref VertexBuffer.
		DataSource *getGlslVertexShaderSourceFor(Shader *shader, VertexBuffer *vbo);

		/// get a suitable fragment shader source for a given \ref VertexBuffer.
		DataSource *getGlslFragmentShaderSourceFor(Shader *shader, VertexBuffer *vbo);

		/// get a suitable vertex shader source for a given \ref VertexBuffer.
		DataSource *getHlslVertexShaderSourceFor(Shader *shader, VertexBuffer *vbo);

		/// get a suitable fragment shader source for a given \ref VertexBuffer.
		DataSource *getHlslFragmentShaderSourceFor(Shader *shader, VertexBuffer *vbo);

	private:
		DataSourceCache			cached_glsl_vertex_shaders;
		DataSourceCache			cached_glsl_fragment_shaders;

		DataSourceCache			cached_hlsl_vertex_shaders;
		DataSourceCache			cached_hlsl_fragment_shaders;

		ShaderCache				cached_shaders;
	};

}
}

#endif	/* __WIESEL_GL_SHADER_SHADERS_H__ */

