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
#ifndef __WIESEL_GRAPH_LIGHTING_LIGHTNODE_2D_H__
#define __WIESEL_GRAPH_LIGHTING_LIGHTNODE_2D_H__

#include <wiesel/wiesel-core.def>
#include "wiesel/graph/2d/node2d.h"

#include "light_source.h"


namespace wiesel {

	/**
	 * @brief An implementation of \ref LightSource based on a \ref Node2D.
	 * This creates a lightsource, which can be moved within a 2D world
	 * by using the Node2D API functions.
	 */
	class WIESEL_CORE_EXPORT LightNode2D :
			public Node2D,
			public LightSource
	{
	public:
		LightNode2D();
		virtual ~LightNode2D();

	public:
		/**
		 * @brief Set the Z position of this light.
		 * This will change the light's distance to the rendered sprites
		 * and therefore the appearance of the lightcone (depending on the shader).
		 */
		void setLightZPosition(float z);

		/**
		 * @brief Get the Z position of this light.
		 */
		inline float getLightZPosition() const {
			return light_z;
		}

	// LightSource
	public:
		virtual vector3d getWorldLocation();

	protected:
		virtual void onDraw(wiesel::video::RenderContext *render_context);

	private:
		float		light_z;
	};

}

#endif // __WIESEL_GRAPH_LIGHTING_LIGHTNODE_2D_H__
