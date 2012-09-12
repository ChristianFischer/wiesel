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
#ifndef __WIESEL_PLATFORM_SDL_ENGINE_H__
#define __WIESEL_PLATFORM_SDL_ENGINE_H__

#include <wiesel/wiesel-common.def>

#ifdef WIESEL_USE_LIBSDL

#include "../../../wiesel/engine.h"


namespace wiesel {

	class GenericFileSystem;


	/**
	 * @brief The SDL engine implementation.
	 */
	class WIESEL_COMMON_EXPORT SdlEngine
	: public Engine
	{
	public:
		SdlEngine();
		virtual ~SdlEngine();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();

		virtual TouchHandler *getTouchHandler();

	public:
		virtual bool decodeImage(
				DataSource *data,
				unsigned char **pBuffer, size_t *pSize,
				unsigned int *pWidth, unsigned int *pHeight,
				unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
				int *pRbits, int *pGbits, int *pBbits, int *pAbits,
				bool as_texture
		);

	protected:
		FileSystem*		root_fs;
		FileSystem*		asset_fs;

		TouchHandler*	touch_handler;
	};

}

#endif /* WIESEL_USE_LIBSDL */
#endif /* __WIESEL_PLATFORM_SDL_ENGINE_H__ */
