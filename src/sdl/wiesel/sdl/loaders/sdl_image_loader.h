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
#ifndef __WIESEL_SDL_LOADERS_SDLIMAGELOADER_H__
#define __WIESEL_SDL_LOADERS_SDLIMAGELOADER_H__

#include <wiesel/io/databuffer.h>
#include <wiesel/io/file.h>
#include <wiesel/modules.h>
#include <wiesel/resources/graphics/image_loader.h>
#include <wiesel/wiesel-sdl.def>


namespace wiesel {

	class WIESEL_SDL_EXPORT SdlImageLoader : public IImageLoader
	{
	private:
		SdlImageLoader();

	public:
		static SdlImageLoader *create();
		
		virtual ~SdlImageLoader();


		virtual Image *loadImage(DataSource *source);
		virtual Image *loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size);
		
	private:
		virtual Image *internal_loadImage(DataSource *source, dimension *pOriginalSize, bool pot);
	};
}

#endif // __WIESEL_SDL_LOADERS_SDLIMAGELOADER_H__
