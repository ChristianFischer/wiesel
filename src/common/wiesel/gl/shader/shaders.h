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
		static const char *UNIFORM_TEXTURE;
		static const char *VARYING_COLOR;
		static const char *VARYING_NORMAL;
		static const char *VARYING_TEXTURE_COORDINATE;

	// vertex buffer shaders
	public:
		/// get a suitable shader for a given \ref VertexBuffer.
		ShaderProgram *getShaderFor(VertexBuffer *vbo);

		/// get a suitable vertex shader for a given \ref VertexBuffer.
		Shader *getVertexShaderFor(VertexBuffer *vbo);

		/// get a suitable fragment shader for a given \ref VertexBuffer.
		Shader *getFragmentShaderFor(VertexBuffer *vbo);
	};

}

#endif	/* __WIESEL_GL_SHADER_SHADERS_H__ */

