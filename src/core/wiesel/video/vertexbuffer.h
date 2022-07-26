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
#ifndef __WIESEL_GL_VBO_VERTEXBUFFER_H__
#define __WIESEL_GL_VBO_VERTEXBUFFER_H__

#include <wiesel/wiesel-core.def>

#include "texture.h"
#include "shader.h"

#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>
#include <wiesel/video/screen.h>
#include <wiesel/device_resource.h>

#include <vector>
#include <string>


namespace wiesel {
namespace video {


	// predeclarations

	class IndexBuffer;
	class VertexBufferContent;



	/**
	 * @brief A class holding configurable data of vertices.
	 * A vertex can contain 2D or 3D coordinates, normal, color,
	 * and multiple texture coordinates.
	 */
	class WIESEL_CORE_EXPORT VertexBuffer : public TDeviceResource<Screen, VertexBufferContent>
	{
	public:
		/// alias type for the index of each vertex.
		typedef unsigned int index_t;

		/// pointer type
		typedef unsigned char* data_t;

		/// description of a single component within the vertex buffer.
		struct component {
			/// the number of fields of the component
			unsigned char	fields;

			/// the total size of this component
			unsigned char	size;

			/// the start offset of this component relative to the vertex beginning
			unsigned char	offset;
		};

	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		/**
		 * @brief configure the vertex position member of the vertices.
		 * @param dimensions	The number of dimensions of each vertex.
		 * 						This must be either 2 or 3 dimensions.
		 */
		bool setupVertexPositions(int dimensions=3);

		/**
		 * @brief configure the normals member of the vertices.
		 */
		bool setupVertexNormals();

		/**
		 * @brief configure the colors member of the vertices.
		 * @param fields	The number of color components for each color.
		 * 					3 for RGB, 4 for RGBA.
		 */
		bool setupVertexColors(int fields=4);

		/**
		 * @brief configure a texture layer of the vertices.
		 * @param layer		The layer to be configured.
		 */
		bool setupTextureLayer(unsigned int layer);

		/**
		 * @brief disables vertex normals.
		 */
		void disableVertexNormals();

		/**
		 * @brief disables the vertex colors.
		 */
		void disableVertexColors();

		/**
		 * @brief disables a texture layer.
		 */
		void disableTextureLayer(unsigned int layer);



		/**
		 * @brief check, if the vertices have positions.
		 */
		bool hasPositions() const;

		/**
		 * @brief check, if the vertices have normals.
		 */
		bool hasNormals() const;

		/**
		 * @brief check, if the vertices have a color component.
		 */
		bool hasColors() const;

		/**
		 * @brief check, if this vertex buffer contains texture data.
		 */
		bool hasTextures() const;

		/**
		 * @brief get the number of vertex dimensions.
		 */
		int getVertexDimensions() const;

		/**
		 * @brief get the number of color components of each vector.
		 */
		int getVertexColorComponents() const;

		/**
		 * @brief get the number of texture layers.
		 */
		int getNumberOfTextureLayers() const;

		/**
		 * @brief get the description of the position component.
		 */
		inline const component& getPositionDescription() const {
			return positions;
		}

		/**
		 * @brief get the description of the normal component.
		 */
		inline const component& getNormalDescription() const {
			return normals;
		}

		/**
		 * @brief get the description of the color component.
		 */
		inline const component& getColorDescription() const {
			return colors;
		}

		/**
		 * @brief get the description of the color component.
		 */
		inline const component& getTextureDescription(unsigned int layer) const {
			return textures[layer];
		}

	public:
		/**
		 * @brief get the name of the default shader for this vertex buffer.
		 * This name will be used for caching the default shaders.
		 */
		std::string getDefaultShaderName() const;


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
		 * @brief adds a vertex with two coordinates.
		 * @return The index of the new vertex.
		 */
		index_t addVertex(const vector2d &v);

		/**
		 * @brief adds a vertex with three coordinates.
		 * @return The index of the new vertex.
		 */
		index_t addVertex(const vector3d &v);

		/**
		 * @brief adds a vertex with three coordinates.
		 * @return The index of the new vertex.
		 */
		index_t addVertex(float x, float y, float z=0.0f);

		/**
		 * @brief set the position of a vertex.
		 * @param index		The index of the vertex to change.
		 */
		void setVertexPosition(index_t index, float x, float y, float z=0);

		/**
		 * @brief set the normal of a vertex.
		 * @param index		The index of the vertex to change.
		 */
		void setVertexNormal(index_t index, float x, float y, float z);

		/**
		 * @brief set the RGB(A) color of a vertex.
		 * @param index		The index of the vertex to change.
		 */
		void setVertexColor(index_t index, float r, float g, float b, float a=1.0f);

		/**
		 * @brief set the first texture layer's coordinate of a vertex.
		 * @param index		The index of the vertex to change.
		 */
		void setVertexTextureCoordinate(index_t index, const vector2d &coord);

		/**
		 * @brief set the first texture layer's coordinate of a vertex.
		 * @param index		The index of the vertex to change.
		 */
		void setVertexTextureCoordinate(index_t index, float u, float v);

		/**
		 * @brief set the texture coordinate of a vertex.
		 * @param index		The index of the vertex to change.
		 * @param layer		The texture layer which coordinate should be set.
		 */
		void setVertexTextureCoordinate(index_t index, unsigned int layer, const vector2d &coord);

		/**
		 * @brief set the texture coordinate of a vertex.
		 * @param index		The index of the vertex to change.
		 * @param layer		The texture layer which coordinate should be set.
		 */
		void setVertexTextureCoordinate(index_t index, unsigned int layer, float u, float v);

		/**
		 * @brief clears the content of this vertex buffer.
		 */
		void clear();

	// data getter
	public:
		/**
		 * @brief Get the pointer to the vertexbuffers data.
		 * The data is meant to be read-only and should be written only by using
		 * the functions provided by the VertexBuffer class.
		 */
		inline const data_t getDataPtr() const {
			return data;
		}

		/**
		 * @brief Get the size of each vertex in bytes.
		 */
		unsigned short getVertexSize() const;

	protected:
		/// checks, if the setup of this vertex buffer can be changed.
		bool checkIfSetupPossible() const;

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();

	private:
		/// compute the offsets of each data member.
		void updateOffsets();

		/// get the pointer of a specific
		data_t getVertexPtr(index_t index, const component &comp) const;

		/// writes a single value into the buffer
		template <typename T>
		void setValue(data_t ptr, size_t offset, T value) {
			*(reinterpret_cast<T*>(ptr + offset)) = value;
		}

		/// Invalidates the hardware buffer, so the buffer needs to be re-created next time.
		void invalidateHardwareData() const;

	private:
		component				positions;
		component				normals;
		component				colors;
		std::vector<component>	textures;
		unsigned short			vertex_size;

		index_t					num_vertices;
		index_t					capacity;

		data_t					data;
	};




	/**
	 * @brief Handles the device specific part of a vertex buffer.
	 */
	class WIESEL_CORE_EXPORT VertexBufferContent : public virtual DeviceResourceContent
	{
	private:
		VertexBufferContent();

	protected:
		VertexBufferContent(VertexBuffer *vertex_buffer);

	public:
		virtual ~VertexBufferContent();

	// public getter
	public:
		/// get the assigned texture
		inline VertexBuffer *getVertexBuffer() {
			return vertex_buffer;
		}

	private:
		VertexBuffer*	vertex_buffer;
	};

} /* namespace video */
} /* namespace wiesel */
#endif /* __WIESEL_VBO_VERTEXBUFFER_H__ */
