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
#ifndef __WIESEL_SDL2_SDLMESSAGERECEIVER_H__
#define __WIESEL_SDL2_SDLMESSAGERECEIVER_H__

#include <wiesel/wiesel-sdl2.def>
#include <wiesel/engine_interfaces.h>
#include <wiesel/platform.h>

#include <SDL.h>

#include <vector>


namespace wiesel {
namespace sdl2 {

	/**
	 * @brief An interface for classes, which wants to receive messages
	 * from the SDL event queue.
	 */
	class ISdl2MessageReceiver :
		public virtual SharedObject
	{
	public:
		ISdl2MessageReceiver();
		virtual ~ISdl2MessageReceiver();

		virtual void onEvent(const SDL_Event &event) = 0;
	};

}
}

#endif /* __WIESEL_SDL2_SDLMESSAGERECEIVER_H__ */
