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
#ifndef __WIESEL_RESOURCES_GRAPHICS_IMAGE_H__
#define __WIESEL_RESOURCES_GRAPHICS_IMAGE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/geometry.h>
#include <wiesel/util/shared_object.h>
#include "wiesel/io/databuffer.h"


namespace wiesel {


	/**
	 * @brief Describes color components and their order for each pixel of an image.
	 */
	enum PixelFormat {
		PixelFormat_RGB_888,
		PixelFormat_RGBA_8888,
	};


	/**
	 * @brief A class containing image data and a description of it.
	 */
	class WIESEL_CORE_EXPORT Image : public virtual SharedObject
	{
	public:
		/**
		 * @brief Creates a new image without data assigned.
		 */
		Image();

		/**
		 * @brief Creates a new image and assigns image data to it.
		 */
		Image(DataBuffer *data, PixelFormat format, const dimension &size);

		virtual ~Image();

	public:
		/**
		 * @brief Get the \ref DataBuffer object containing the image data.
		 */
		inline DataBuffer *getPixelData() const {
			return pixel_data;
		}
		
		/**
		 * @brief Get the current pixel format of this image.
		 */
		inline PixelFormat getPixelFormat() const {
			return pixel_format;
		}

		/**
		 * @brief Get the size of this image.
		 */
		inline const dimension& getSize() const {
			return image_size;
		}

	public:
		/**
		 * @brief Assigns new image data to this image object.
		 * Before the data will be assigned, it will be checked if the buffer size matches
		 * pixel format and image size.
		 * @param data		A \ref DataBuffer containing the pixel data.
		 * @param format	The pixel format of the assigned pixel data.
		 * @param size		The size of the image.
		 * @return \c true, if the data was successfully assigned, \c false otherwise.
		 */
		virtual bool assignImageData(DataBuffer *data, PixelFormat format, const dimension &size);

		/**
		 * @brief Change the pixel format of this image.
		 * The image will be converted into the new format, which may take a few milliseconds.
		 * @return \c true, if the operation was successfull, \c false otherwise.
		 */
		virtual bool changePixelFormat(PixelFormat new_pixel_format);

		/**
		 * @brief Change the image's size.
		 * The DataBuffer will be converted into the new size, which may take a few milliseconds.
		 * When the size is decreased, the right and bottom parts of the image will be lost.
		 * When increasing the image, the original image is aligned on the top left corner,
		 * and the bottom right part will be filled with black.
		 * @return \c true, if the operation was successfull, \c false otherwise.
		 */
		virtual bool resize(const dimension &new_size);

		/**
		 * @brief Checks, if the image's size is a power-of-two and resizes the image
		 * if neccessary.
		 */
		virtual bool ensurePowerOfTwo();

	private:
		DataBuffer*		pixel_data;
		PixelFormat		pixel_format;
		dimension		image_size;
	};
}

#endif
