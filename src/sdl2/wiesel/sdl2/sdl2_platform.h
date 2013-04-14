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
#ifndef __WIESEL_SDL2_SDLPLATFORM_H__
#define __WIESEL_SDL2_SDLPLATFORM_H__

#include <wiesel/wiesel-sdl2.def>
#include <wiesel/engine_interfaces.h>
#include <wiesel/platform.h>

#include <vector>


#define WIESEL_SDL_LOG_TAG "SDL"


namespace wiesel {
namespace sdl2 {

	class Sdl2Platform;
	class ISdl2MessageReceiver;


	/**
	 * @brief The main class of all SDL modules.
	 * This class manages the SDL message loop and delivers
	 * the events to all available SDL-modules.
	 */
	class WIESEL_SDL2_EXPORT Sdl2Platform : public Platform
	{
	public:
		Sdl2Platform();
		virtual ~Sdl2Platform();

	public:
		/// registers a new message receiver
		void addReceiver(ISdl2MessageReceiver *recv);

		/// removes an existing message receiver
		void removeReceiver(const ISdl2MessageReceiver *recv);

	// overridables
	public:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();
		virtual FileSystem *getDataFileSystem(const std::string &subdir);
		virtual FileSystem *getExternalDataFileSystem(const std::string &subdir);

	private:
		std::vector<ISdl2MessageReceiver*>	receivers;
	};

}
}

#endif /* __WIESEL_SDL2_SDLPLATFORM_H__ */
