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
#ifndef __WIESEL_RESOURCES_IMAGES_IMAGELOADER_H__
#define __WIESEL_RESOURCES_IMAGES_IMAGELOADER_H__

#include <wiesel/wiesel-core.def>

#include "wiesel/module.h"
#include "wiesel/io/datasource.h"
#include "image.h"


namespace wiesel {


	/**
	 * @brief An interface for a module for loading images from a data source.
	 */
	class WIESEL_CORE_EXPORT IImageLoader : public Module
	{
	public:
		IImageLoader() {}
		virtual ~IImageLoader() {}

	public:
		/**
		 * @brief Loads a new image from a data source.
		 */
		virtual Image *loadImage(DataSource *source) = 0;

		/**
		 * @brief Loads a new image from a data source.
		 * The image should be created with a size which is a power of two.
		 * @param source			The data source of the image.
		 * @param original_size		Pointer to a dimension-object, which will receive
		 *							the original size of the image, if the size was increased
		 *							to match a power of two value.
		 * @return The image which was loaded from the data source or \c NULL, when the loading failed.
		 */
		virtual Image *loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size) = 0;
	};

}

#endif // __WIESEL_RESOURCES_IMAGES_IMAGELOADER_H__
