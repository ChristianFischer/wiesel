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
#ifndef __WIESEL_VIDEO_SDL2_GL_RENDERCONTEXT_H__
#define	__WIESEL_VIDEO_SDL2_GL_RENDERCONTEXT_H__

#include <wiesel/wiesel-sdl2.def>

#include <wiesel/video/gl/gl_render_context.h>


namespace wiesel {
namespace sdl2 {


	/**
	 * @brief SDL version of the OpenGL render context.
	 */
	class WIESEL_SDL2_EXPORT Sdl2GlRenderContext : public wiesel::video::gl::OpenGlRenderContext
	{
	public:
		Sdl2GlRenderContext(wiesel::video::Screen *screen);
		virtual ~Sdl2GlRenderContext();

	public:
		virtual void preRender();
		virtual void postRender();
	};

}
}

#endif	// __WIESEL_VIDEO_SDL2_GL_RENDERCONTEXT_H__
