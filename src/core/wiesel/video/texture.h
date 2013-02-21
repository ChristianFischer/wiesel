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
#ifndef __WIESEL_GL_TEXTURE_TEXTURE_H__
#define __WIESEL_GL_TEXTURE_TEXTURE_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/geometry.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/device_resource.h>

#include <string>


namespace wiesel {
namespace video {

	class Screen;
	class TextureContent;



	class WIESEL_CORE_EXPORT Texture : public TDeviceResource<Screen, TextureContent>
	{
	private:
		Texture();

	public:
		virtual ~Texture();

		/**
		 * @brief load a texture from a \ref File.
		 */
		static WIESEL_CORE_EXPORT Texture *fromFile(File *filename);

		/**
		 * @brief load a texture from a \ref DataSource.
		 */
		static WIESEL_CORE_EXPORT Texture *fromDataSource(DataSource *data);

		/**
		 * @brief Get the datasource, where we get the texture's data from.
		 */
		inline DataSource *getSource() {
			return data;
		}

		/**
		 * @brief get the texture's size.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getSize() const {
			return size;
		}

		/**
		 * @brief get the texture's original size, as it was stored in the texture image.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getOriginalSize() const {
			return original_size;
		}

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();

	private:
		DataSource*		data;
		dimension		size;
		dimension		original_size;
	};




	/**
	 * @brief Handles the device specific part of a texture.
	 */
	class WIESEL_CORE_EXPORT TextureContent : public virtual DeviceResourceContent
	{
	private:
		TextureContent();

	protected:
		TextureContent(Texture *texture);

	public:
		virtual ~TextureContent();

	// public getter
	public:
		/// get the assigned texture
		inline Texture *getTexture() {
			return texture;
		}

		/**
		 * @brief get the texture's size.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getSize() const {
			return size;
		}

		/**
		 * @brief get the texture's original size, as it was stored in the texture image.
		 * The actual size of the texture may differ from the texture-file's size,
		 * when the hardware doesn't support the file's dimension.
		 */
		inline const dimension& getOriginalSize() const {
			return original_size;
		}

	protected:
		dimension		size;
		dimension		original_size;

	private:
		Texture*		texture;
	};

} /* namespace video */
} /* namespace wiesel */
#endif /* __WIESEL_GL_TEXTURE_TEXTURE_H__ */
