/*
 * vertexbuffer.h
 *
 *  Created on: 30.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_GL_VBO_VERTEXBUFFER_H__
#define __WIESEL_GL_VBO_VERTEXBUFFER_H__

#include <wiesel/gl/gl.h>
#include <wiesel/gl/shader/shader.h>

#include <vector>


namespace wiesel {

	/**
	 * @brief A class holding configurable data of vertices.
	 * A vertex can contain 2D or 3D coordinates, normal, color,
	 * and multiple texture coordinates.
	 */
	class VertexBuffer
	{
	public:
		/// alias type for the index of each vertex.
		typedef unsigned int index_t;

		VertexBuffer();
		virtual ~VertexBuffer();

		/**
		 * @brief adds a vertex with two coordinates.
		 * @return The index of the new vertex.
		 */
		index_t addVertex(float x, float y);

		/**
		 * @brief adds a vertex with three coordinates.
		 * @return The index of the new vertex.
		 */
		index_t addVertex(float x, float y, float z);

		/**
		 * @brief bind this vertex buffer to a shader
		 * and link each shader attribute to the correct data pointer.
		 * @return true, when each field of this buffer was successfully bound to a shader attribute.
		 */
		bool bind(const ShaderProgram *shader);

		/**
		 * @brief renders this vertex buffer.
		 */
		void render();

	protected:

	private:
		std::vector<float>	data;

	};

} /* namespace wiesel */
#endif /* __WIESEL_VBO_VERTEXBUFFER_H__ */
