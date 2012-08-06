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
#ifndef __WIESEL_GL_VBO_INDEXBUFFER_H__
#define __WIESEL_GL_VBO_INDEXBUFFER_H__

#include <wiesel/gl/gl.h>
#include <wiesel/util/shared_object.h>
#include <wiesel/gl/shader/shader.h>

#include "vertexbuffer.h"

#include <vector>


namespace wiesel {

	/**
	 * @brief 
	 */
	class IndexBuffer : public virtual SharedObject
	{
	public:
		/// alias type for the index of each vertex.
		typedef unsigned int index_t;

		IndexBuffer();

		IndexBuffer(int size);

		virtual ~IndexBuffer();


		/**
		 * @brief Configure the size of each index entry.
		 * The size cannot be altered, when the index buffer already contains data.
		 * The index buffer needs to be cleared to alter the entry size.
		 * @param bytes
		 *		Possible values:
		 *		<ul><li>1 - Supports a maximum of 256 entries.</li>
		 *			<li>2 - Supports a maximum of 65.536 entries.</li>
		 *			<li>1 - Supports a maximum of 4.294.967.296 entries.</li>
		 *		</ul>
		 */
		void setBytesPerElement(int bytes);

		/**
		 * @brief Get the size in bytes for each index entry.
		 */
		int getBytesPerElement() const;

		/**
		 * @brief set the capacity of this vertex buffer to store the given amount of vertices.
		 * The capacity cannot be lower than the current size of the buffer.
		 * @brief returns the new capacity.
		 */
		index_t setCapacity(index_t capacity);

		/**
		 * @brief ensures this vertex buffer can store at least the given amount of vertices.
		 * Increases the size of the buffer if necessary.
		 * @brief returns the new capacity.
		 */
		index_t ensureCapacity(index_t capacity);

		/**
		 * @brief get the current capacity of this vertex buffer.
		 */
		index_t getCapacity() const;

		/**
		 * @brief get the number of vertices currently stored in the buffer.
		 */
		index_t getSize() const;



		/**
		 * @brief adds a new index to this buffer.
		 * @return The index of the new index.
		 */
		index_t addIndex(VertexBuffer::index_t index);

		/**
		 * @brief set the value at a specific index.
		 * @param index		The index of the IndexBuffer value to be set.
		 * @param value		The index-value to be set.
		 */
		void setIndex(index_t index, VertexBuffer::index_t value);

		/**
		 * @brief clears the content of this vertex buffer.
		 */
		void clear();

		/**
		 * @brief bind this vertex buffer to a shader
		 * and link each shader attribute to the correct data pointer.
		 * This method does not bind any textures.
		 * @param program	The shader program used to render this vertex buffer.
		 * @return true, when each field of this buffer was successfully bound to a shader attribute.
		 */
		bool bind() const;

		/**
		 * @brief removes the bindings of this vertex buffer.
		 */
		void unbind() const;
		
	private:
		/**
		 * @brief Invalidates the hardware buffer, so the buffer needs to be re-created next time.
		 */
		void invalidateHardwareData() const;

	private:
		/// pointer type
		typedef unsigned char* data_t;

		mutable GLuint			handle;

		int						bytes_per_entry;

		index_t					num_entries;
		index_t					capacity;

		data_t					data;
	};

} /* namespace wiesel */
#endif /* __WIESEL_GL_VBO_INDEXBUFFER_H__ */
