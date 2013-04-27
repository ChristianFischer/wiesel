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
#ifndef __WIESEL_GRAPH_VIEWPORT_H__
#define	__WIESEL_GRAPH_VIEWPORT_H__

#include <wiesel/wiesel-core.def>

#include "2d/node2d.h"

#include <wiesel/geometry.h>

#include <stdint.h>


namespace wiesel {

	/**
	 * @brief A base-class for nodes, which provides a specific content
	 * area for their children.
	 */
	class WIESEL_CORE_EXPORT Viewport : public Node2D
	{
	public:
		enum Flags {
			/**
			 * @brief With this option, the viewport inherits the original size
			 * from it's parent and does not perform any rescaling.
			 */
			Inherit						= 0x0001,

			/**
			 * @brief Scales the viewport to the given width value.
			 * The height value will be computed from the parent's aspect ratio.
			 */
			ScaleByWidth				= 0x0100,

			/**
			 * @brief Scales the viewport to the given height value.
			 * The width value will be computed from the parent's aspect ratio.
			 */
			ScaleByHeight				= 0x0200,

			/**
			 * @brief Scales the viewport either to width or height,
			 * whatever the smallest scale factor is.
			 * When the aspect ratio of the parent does not match the requested ratio,
			 * the smaller dimension will get borders, depending on the requested align.
			 */
			ScaleByWidthOrHeight		= ScaleByWidth | ScaleByHeight,

			/**
			 * @brief When the width of the viewport is lower than it's parent,
			 * the width's value will be increased to fill the screen.
			 */
			ExtendWidth					= 0x1000,

			/**
			 * @brief When the height of the viewport is lower than it's parent,
			 * the height's value will be increased to fill the screen.
			 */
			ExtendHeight				= 0x2000,

			/**
			 * @brief Extend width and height of the viewport to match the parents size.
			 */
			ExtendWidthAndHeight		= ExtendWidth | ExtendHeight,

			/**
			 * @brief Resize the viewport to fit into it's parent and extend the smallest dimension.
			 */
			FitToParent					= ScaleByWidthOrHeight | ExtendWidthAndHeight,
		};

	public:
		/**
		 * @brief Find the next viewport from a given node.
		 */
		static Viewport* findFrom(Node *node);

	public:
		Viewport();
		virtual ~Viewport();

	public:
		/**
		 * @brief Set the viewport's scale mode.
		 */
		void setScaleMode(uint32_t flags, const dimension& dim = dimension(0, 0), const vector2d& align = vector2d::zero);

		/**
		 * @brief Get the viewport's current scale mode.
		 */
		inline uint32_t getViewportFlags() const {
			return viewport_flags;
		}

		/**
		 * @brief Get the viewport's current scale mode dimension.
		 */
		inline const dimension& getViewportRequestedDimension() const {
			return viewport_requested_dimension;
		}

		/**
		 * @brief Get the current viewport.
		 * NOTE: Children can ignore viewport.position, because {0,0} of the coordinate system
		 * is already translated by the viewports position.
		 */
		inline const rectangle& getViewport() const {
			return viewport;
		}

		/**
		 * @brief Get the viewport's size.
		 */
		inline const dimension& getViewportSize() const {
			return viewport.size;
		}

	protected:
		virtual void computeLocalTransform(matrix4x4 *transform);

		/**
		 * @brief Get the parent's viewport.
		 */
		virtual rectangle getParentViewport();

	// overridables
	protected:
		/**
		 * @brief Notification when this viewport's size has changed.
		 */
		virtual void onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport);

	private:
		uint32_t		viewport_flags;
		vector2d		viewport_align;
		dimension		viewport_requested_dimension;

		rectangle		viewport;
	};

}

#endif	/* __WIESEL_GRAPH_VIEWPORT_H__ */

