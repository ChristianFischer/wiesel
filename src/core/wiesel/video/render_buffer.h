/**
 * Copyright (C) 2013
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
#ifndef __WIESEL_VIDEO_RENDER_BUFFER_H__
#define __WIESEL_VIDEO_RENDER_BUFFER_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/device_resource.h>

#include "texture.h"

#include <stdint.h>
#include <vector>


namespace wiesel {
namespace video {

	class RenderBuffer;
	class RenderBufferContent;
	class RenderContext;


	/**
	 * @brief The \c RenderBuffer class can be used to redirect drawings into one or more
	 * target textures.
	 */
	class WIESEL_CORE_EXPORT RenderBuffer : public TDeviceResource<Screen, RenderBufferContent>
	{
	public:
		struct TextureEntry {
			Texture*	texture;
		};

		typedef std::vector<TextureEntry>	TextureList;

	public:
		RenderBuffer();

		RenderBuffer(Texture *texture);

		virtual ~RenderBuffer();

	public:
		/**
		 * @brief Set the \ref RenderTarget's viewport.
		 */
		void setViewport(const rectangle& viewport);

		/**
		 * @brief Set the \ref RenderTarget's viewport.
		 */
		void setViewport(const dimension& size);

		/**
		 * @brief Set the \ref RenderTarget's viewport.
		 */
		void setViewport(float x, float y, float width, float height);

		/**
		 * @brief Get the \ref RenderTarget's viewport.
		 */
		inline const rectangle& getViewport() const {
			return viewport;
		}

		/**
		 * @brief Set a target texture for a specific index.
		 * @param index		The index for the new target texture.
		 * @param texture	The new texture, or \c NULL to clear the previous one.
		 */
		void setTargetTexture(uint8_t index, Texture *texture);

		/**
		 * @brief Get the target texture at a specific index.
		 * @param index		The target texture's index.
		 * @returns The target texture at the given index, or \c NULL,
		 *			when this index had no target texture.
		 */
		Texture* getTargetTexture(uint8_t index) const;

		/**
		 * @brief Get the read-only list of target textures.
		 */
		inline const TextureList* getTargetTextures() const {
			return &target_textures;
		}

		/**
		 * @brief Get the number of target textures currently configured.
		 */
		inline uint8_t getTargetTextureCount() const {
			return target_textures.size();
		}

		/**
		 * @brief Clears all target textures from this buffer.
		 */
		void clearTextures();

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();

	private:
		rectangle			viewport;

		TextureList			target_textures;
	};




	class WIESEL_CORE_EXPORT RenderBufferContent : public virtual DeviceResourceContent
	{
	private:
		RenderBufferContent();

	protected:
		RenderBufferContent(RenderBuffer *render_target);

	public:
		virtual ~RenderBufferContent();

	// public getter
	public:
		/// get the assigned render target
		inline RenderBuffer *getRenderBuffer() {
			return render_buffer;
		}

	// rendering
	public:
		/**
		 * @brief Enables this renderbuffer to receive all following rendering operations.
		 * After calling this function, all following draw calls should be
		 * drawn into the target textures until \ref disableRenderBuffer() was called.
		 * @returns	\c true, when the renderbuffer is active, \c false when the renderbuffer
		 *			could not be enabled. The renderbuffer must still be disabled, otherwise
		 *			follwoing draw calls may get lost.
		 */
		virtual bool enableRenderBuffer(RenderContext *render_context) = 0;

		/**
		 * @brief Disables this renderbuffer, so all rendering operations will be drawn into
		 * the previous render target.
		 */
		virtual void disableRenderBuffer(RenderContext *render_context) = 0;

		/**
		 * @brief Called before rendering starts to allow initialization of this buffer.
		 */
		virtual void preRender(RenderContext *render_context) = 0;

		/**
		 * @brief Called after the rendering into this buffer has been completed.
		 */
		virtual void postRender(RenderContext *render_context) = 0;

	private:
		RenderBuffer*	render_buffer;
	};

}
}

#endif // __WIESEL_VIDEO_RENDER_BUFFER_H__
