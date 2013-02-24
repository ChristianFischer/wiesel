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
#ifndef __WIESEL_VIDEO_RENDER_CONTEXT_H__
#define	__WIESEL_VIDEO_RENDER_CONTEXT_H__

#include "wiesel/wiesel-core.def"
#include "screen.h"
#include "shader.h"
#include "types.h"

#include <wiesel/device.h>
#include <wiesel/geometry.h>

#include <inttypes.h>
#include <string>


namespace wiesel {
namespace video {


	class IndexBuffer;
	class Shader;
	class Texture;
	class VertexBuffer;



	/**
	 * @brief This class handles the all prcesses required for rendering objects to the screen.
	 */
	class WIESEL_CORE_EXPORT RenderContext : public Device
	{
	private:
		RenderContext();

	protected:
		RenderContext(Screen *screen);

	public:
		virtual ~RenderContext();

		/**
		 * @brief Initialize the render context.
		 */
		virtual void initContext() = 0;

	// getter
	public:
		/**
		 * @brief Get the screen object, which is related to this render context.
		 */
		inline Screen *getScreen() {
			return screen;
		}

		/**
		 * @brief Get the renderer's projection matrix.
		 */
		inline const matrix4x4& getProjectionMatrix() const {
			return projection;
		}

	// set values
	public:
		/**
		 * @brief Stores the current projection matrix.
		 * The projection matrix will be kept persistent during the render cycle.
		 * If there's a shader program attached, the shader will receive the new projection matrix.
		 * All upcoming shaders will receive the projection matrix automatically.
		 */
		virtual void setProjectionMatrix(const matrix4x4 &matrix) = 0;

		/**
		 * @brief Send the modelview matrix to the current shader.
		 * If there's no shader attached, nothing will happen. Upcoming shaders will NOT receive
		 * the new modelview-matrix. After setting the shader, the modelview matrix needs to be
		 * sent again by the calling program.
		 */
		virtual void setModelviewMatrix(const matrix4x4 &matrix) = 0;


	// handling resources
	public:
		/**
		 * @brief Set the active shader program.
		 * @param shader	Reference to a shader program. May be \c NULL
		 *					to clear the current shader.
		 */
		virtual void setShader(Shader *shader) = 0;

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param type		The attribute's type.
		 * @param elements	The number of elements within the parameter, to use an array of values.
		 * @param pValue	Pointer to the value, which will be delivered to the shader.
		 */
		virtual void setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, void *pValue) = 0;

		/**
		 * @brief Set a texture for a specific texture unit.
		 * @param index		The index of the texture unit to set.
		 *					Needs to be smaller than the texture unit maximum.
		 * @param texture	The texture to assign to the selected texture unit.
		 *					May be \c NULL to clear the texture unit.
		 */
		virtual void setTexture(uint16_t index, Texture *texture) = 0;

		/**
		 * @brief Clears all active textures.
		 */
		virtual void clearTextures() = 0;

		/**
		 * @brief Prepares the render context for using the given amount of texture layers.
		 * The application should now be able to use their requested number of texture layers.
		 * Each higher layer will be disabled. The prepared layers may still contain other
		 * textures, so it's requred to call setTexture for each requested layer.
		 */
		virtual void prepareTextureLayers(uint16_t layers) = 0;

	// draw primitives
	public:
		/**
		 * @brief Draws some primitives from the data contained in the given vertex buffer.
		 * @param primitive		The type of primitive which should be drawn using the vertex data.
		 * @param vertices		A vertex buffer containing the vertex data to be drawn.
		 */
		virtual void draw(Primitive primitive, const VertexBuffer *vertices) = 0;

		/**
		 * @brief Draws some primitives from the data contained in the given vertex buffer.
		 * @param primitive		The type of primitive which should be drawn using the vertex data.
		 * @param vertices		A vertex buffer containing the vertex data to be drawn.
		 * @param indices		An index buffer containing the indices of the vertices, which should be drawn.
		 */
		virtual void draw(Primitive primitive, const VertexBuffer *vertices, const IndexBuffer *indices) = 0;

	// pre/post-rendering
	public:
		virtual void preRender() = 0;
		virtual void postRender() = 0;

	// callbacks
	public:
		virtual void onSizeChanged(const dimension &size) = 0;

	protected:
		Screen*			screen;
		matrix4x4		projection;
	};

}
}

#endif	// __WIESEL_VIDEO_RENDER_CONTEXT_H__
