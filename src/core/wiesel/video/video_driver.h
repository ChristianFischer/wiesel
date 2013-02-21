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
#ifndef __WIESEL_VIDEO_VIDEODRIVER_H__
#define __WIESEL_VIDEO_VIDEODRIVER_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/math/matrix.h>
#include <wiesel/video/render_context.h>
#include <wiesel/device.h>
#include <wiesel/geometry.h>

#include "screen.h"

#include <ostream>

namespace wiesel {
namespace video {


	class IndexBuffer;
	class IndexBufferContent;
	class Shader;
	class ShaderContent;
	class Texture;
	class TextureContent;
	class VertexBuffer;
	class VertexBufferContent;




	/**
	 * @brief Contains informations about the current \ref VideoDriver.
	 */
	class WIESEL_CORE_EXPORT VideoInfo
	{
	public:
		VideoInfo();
		~VideoInfo();

	public:
		/// the render API name, like OpenGL or DirectX
		std::string		api;

		/// a detailed string describing the version of the rendering api
		std::string		api_version;

		/// the api vendor's name
		std::string		api_vendor;

		/// the renderer's name
		std::string		renderer;

		/// contains various texture informations
		struct _textures {
			/// the maximum texture size for width or height
			uint32_t	max_size;

			/// the maximum number of texture units per drawing call
			uint32_t	max_texture_units;

			/// when true, texture sizes are required to be a power of two (1024, 2048, ...)
			bool		requires_pot;
		} textures;

		/// contains varios shader informations
		struct _shaders {
			/// the shader's API (for example: GLSL or HLSL)
			std::string	api;

			/// the shader's API version
			std::string	api_version;
		} shaders;

		std::vector<std::string>	extensions;
	};

	WIESEL_CORE_EXPORT std::ostream& operator <<(std::ostream &o, const VideoInfo &video_info);



	/**
	 * @brief An abstract interface to a video output device.
	 */
	class WIESEL_CORE_EXPORT VideoDeviceDriver : public DeviceDriver
	{
	private:
		VideoDeviceDriver();

	public:
		VideoDeviceDriver(Screen *screen);
		virtual ~VideoDeviceDriver();

		/// Get some information about the current video device.
		inline const VideoInfo *getVideoInfo() const {
			return &info;
		}

		/// get the device's current state
		inline VideoState getState() const {
			return state;
		}

		/// get the current resolution in pixels
		inline const dimension& getResolution() const {
			return size;
		}

		/// get the screen, this video device is currently assigned to
		inline Screen *getScreen() {
			return screen;
		}

		/// get the current aspect ratio of the screen
		inline float getAspectRatio() const {
			return size.width / size.height;
		}

		/// get the renderer's projection matrix
		inline const matrix4x4& getProjectionMatrix() const {
			return projection;
		}

	// overridables
	public:
		/// Converts a position vector from screen coordinates into world coordinates.
		virtual vector2d convertScreenToWorld(const vector2d &world) const = 0;

		/// called before rendering a frame
		virtual void preRender() = 0;

		/// called after rendering a frame
		virtual void postRender() = 0;

		/// get an instance of RenderContext. This instance may only be valid between preRender and postRender.
		virtual RenderContext* getCurrentRenderContext() = 0;

	// resource management
	public:
		/// create a indexbuffer resource content object
		virtual IndexBufferContent *createIndexBufferContent(IndexBuffer *index_buffer) = 0;

		/// create a vertexbuffer resource content object
		virtual VertexBufferContent *createVertexBufferContent(VertexBuffer *vertex_buffer) = 0;

		/// create a shader resource content object
		virtual ShaderContent *createShaderContent(Shader *shader) = 0;

		/// create a texture resource content object
		virtual TextureContent *createTextureContent(Texture *texture) = 0;

	// member functions
	protected:
		/// set the device's current video state
		void setState(VideoState state);

		/// update the device's screen size
		virtual void updateScreenSize(float w, float h);

	protected:
		VideoInfo	info;
		VideoState	state;
		Screen*		screen;
		matrix4x4	projection;
		dimension	size;
	};

}
}

#endif /* __WIESEL_VIDEO_VIDEODRIVER_H__ */
