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
#ifndef __WIESEL_RESOURCES_IMAGES_IMAGELOADER_LIBPNG_H__
#define __WIESEL_RESOURCES_IMAGES_IMAGELOADER_LIBPNG_H__

#include <wiesel/io/databuffer.h>
#include <wiesel/io/file.h>
#include <wiesel/module.h>
#include <wiesel/resources/graphics/image_loader.h>
#include <wiesel/wiesel-common-config.h>
#include <wiesel/wiesel-common.def>


#if WIESEL_SUPPORTS_LIBPNG

namespace wiesel {

	class WIESEL_COMMON_EXPORT LibPngImageLoader : public IImageLoader
	{
	private:
		LibPngImageLoader();

	public:
		static LibPngImageLoader *create();

		virtual ~LibPngImageLoader();


		virtual Image *loadImage(DataSource *source);
		virtual Image *loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size);

	private:
		virtual Image *internal_loadImage(DataSource *source, dimension *pOriginalSize, bool pot);
	};
}

#endif // WIESEL_SUPPORTS_LIBPNG

#endif // __WIESEL_RESOURCES_IMAGES_IMAGELOADER_LIBPNG_H__
