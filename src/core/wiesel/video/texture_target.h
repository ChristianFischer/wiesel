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
#ifndef __WIESEL_VIDEO_TEXTURE_TARGET_H__
#define __WIESEL_VIDEO_TEXTURE_TARGET_H__

#include <wiesel/util/shared_object.h>
#include <wiesel/wiesel-core.def>

#include "texture.h"

#include <stdint.h>


namespace wiesel {
namespace video {


	class RenderContext;
	class Texture;


	/**
	 * @brief This is the baseclass for any class which uses textures.
	 * Implementing classes should not use \ref TextureTarget directly,
	 * but either \ref SingleTextureTarget or \ref MultiTextureTarget.
	 */
	class WIESEL_CORE_EXPORT TextureTarget : public virtual SharedObject
	{
	protected:
		TextureTarget();
		virtual ~TextureTarget();

	public:
		/**
		 * @brief Set a texture for a specific texture unit.
		 * @param index		The index of the texture unit to set.
		 *					Needs to be smaller than the texture unit maximum.
		 * @param texture	The texture to assign to the selected texture unit.
		 *					May be \c NULL to clear the texture unit.
		 */
		virtual void setTexture(uint16_t index, Texture *texture) = 0;

		/**
		 * @brief Get the texture on a specific texture unit.
		 * @return	The texture at the given texture unit or \c NULL,
		 *			when this texture unit does not exist.
		 */
		virtual Texture* getTexture(uint16_t index) = 0;

		/**
		 * @brief Get the texture on a specific texture unit.
		 * @return	The texture at the given texture unit or \c NULL,
		 *			when this texture unit does not exist.
		 */
		virtual const Texture* getTexture(uint16_t index) const = 0;

		/**
		 * @brief Get the current number of texture layers.
		 */
		virtual uint16_t getTextureLayers() const = 0;

		/**
		 * @brief Get the maximum number of texture layers allowed for this object.
		 */
		virtual uint16_t getTextureLayersMax() const = 0;

		/**
		 * @brief Applies the stored textures to the given \ref RenderContext.
		 */
		virtual void applyTextureConfigTo(RenderContext *rc) = 0;

	protected:
		/**
		 * @brief Notification when a texture of this object has changed.
		 * @param index			Index of the assigned texture.
		 * @param old_texture	The old texture, which was replaced.
		 * @param new_texture	The new texture.
		 */
		virtual void onTextureChanged(uint16_t index, Texture* old_texture, Texture* new_texture);
	};




	class WIESEL_CORE_EXPORT SingleTextureTarget : public TextureTarget
	{
	protected:
		SingleTextureTarget();
		virtual ~SingleTextureTarget();

	public:
		/**
		 * @brief Set the texture to be used for this object.
		 */
		void setTexture(Texture* texture);

		/**
		 * @brief Get the texture used for this object.
		 */
		inline Texture* getTexture() {
			return texture;
		}

		/**
		 * @brief Get the texture used for this object.
		 */
		inline const Texture* getTexture() const {
			return texture;
		}

	// TextureTarget
	public:
		virtual void setTexture(uint16_t index, Texture *texture);
		virtual Texture* getTexture(uint16_t index);
		virtual const Texture* getTexture(uint16_t index) const;
		virtual uint16_t getTextureLayers() const;
		virtual uint16_t getTextureLayersMax() const;
		virtual void applyTextureConfigTo(RenderContext *rc);

	private:
		ref<Texture>	texture;
	};




	class WIESEL_CORE_EXPORT MultiTextureTarget : public TextureTarget
	{
	protected:
		MultiTextureTarget();
		virtual ~MultiTextureTarget();

	// TextureTarget
	public:
		virtual void setTexture(uint16_t index, Texture *texture);
		virtual Texture* getTexture(uint16_t index);
		virtual const Texture* getTexture(uint16_t index) const;
		virtual uint16_t getTextureLayers() const;
		virtual void applyTextureConfigTo(RenderContext *rc);

	private:
		ref<Texture>::vector	textures;
	};

}
}

#endif // __WIESEL_VIDEO_TEXTURE_TARGET_H__
