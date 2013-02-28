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
#ifndef __WIESEL_SDL2_VIDEO_SDLVIDEODEVICE_H__
#define __WIESEL_SDL2_VIDEO_SDLVIDEODEVICE_H__

#include <wiesel/wiesel-sdl2.def>

#include <wiesel/geometry.h>
#include <wiesel/video/screen.h>
#include <wiesel/video/gl/gl_video_driver.h>

#include "sdl2_gl_rendercontext.h"

#include "../sdl2_platform.h"
#include "../sdl2_message_receiver.h"


namespace wiesel {
namespace sdl2 {

	/**
	 * @brief The SDL video device implementation.
	 */
	class WIESEL_SDL2_EXPORT Sdl2VideoDeviceDriver :
			public wiesel::video::gl::OpenGlVideoDeviceDriver,
			public ISdl2MessageReceiver
	{
	public:
		Sdl2VideoDeviceDriver(Sdl2Platform *platform, ::wiesel::video::Screen *screen);
		virtual ~Sdl2VideoDeviceDriver();

		bool init(const dimension &size, unsigned int flags);
		bool shutdown();

		virtual void preRender();
		virtual void postRender();

		virtual wiesel::video::RenderContext* getCurrentRenderContext();

	// ISdlMessageReceiver
	public:
		virtual void onEvent(const SDL_Event &event);

	private:
		Sdl2Platform*				platform;
		Sdl2GlRenderContext*		render_context;
		wiesel::video::Screen*		screen;

		SDL_Window*					displayWindow;
		SDL_Renderer*				displayRenderer;
		SDL_RendererInfo			displayRendererInfo;
	};

}
}

#endif // __WIESEL_SDL2_VIDEO_SDLVIDEODEVICE_H__
