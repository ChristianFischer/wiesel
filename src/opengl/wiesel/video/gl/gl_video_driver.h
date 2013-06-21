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
#ifndef __WIESEL_VIDEO_GL_VIDEO_DEVICE_H__
#define __WIESEL_VIDEO_GL_VIDEO_DEVICE_H__

#include <wiesel/wiesel-opengl.def>

#include <wiesel/video/screen.h>
#include <wiesel/video/video_driver.h>


namespace wiesel {
namespace video {
namespace gl {

	/**
	 * @brief The SDL video device implementation.
	 */
	class WIESEL_OPENGL_EXPORT OpenGlVideoDeviceDriver :
			public wiesel::video::VideoDeviceDriver
	{
	public:
		OpenGlVideoDeviceDriver(wiesel::video::Screen *screen);
		virtual ~OpenGlVideoDeviceDriver();
		
	protected:
		bool initOpenGl();

	public:
		virtual vector2d convertScreenToWorld(const vector2d &screen) const;

		virtual void preRender();
		virtual void postRender();

	// resource management
	public:
		virtual IndexBufferContent *createIndexBufferContent(IndexBuffer *index_buffer);
		virtual VertexBufferContent *createVertexBufferContent(VertexBuffer *index_buffer);
		virtual ShaderContent *createShaderContent(Shader *shader);
		virtual ShaderConstantBufferContent *createConstantBufferContent(ShaderConstantBuffer *shader_constant_buffer);
		virtual TextureContent *createTextureContent(Texture *texture);
		virtual RenderBufferContent *createRenderBufferContent(RenderBuffer *render_buffer);
	};

}
}
}

#endif // __WIESEL_VIDEO_GL_VIDEO_DEVICE_H__
