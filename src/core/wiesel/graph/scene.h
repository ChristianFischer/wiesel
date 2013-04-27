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
#ifndef __WIESEL_GRAPH_SCENE_H__
#define	__WIESEL_GRAPH_SCENE_H__

#include <wiesel/wiesel-core.def>

#include "node.h"
#include "viewport.h"

namespace wiesel {

	namespace video {
		class Screen;
	}

	class Scene;

	/**
	 * @brief Alias type for a list of scenes.
	 */
	typedef std::vector<Scene*>		SceneList;


	/**
	 * @brief Base-class for scenes.
	 */
	class WIESEL_CORE_EXPORT Scene : public Viewport
	{
	public:
		Scene();
		virtual ~Scene();

	// Node
	public:
		virtual void render(video::RenderContext *render_context);

	// Viewport
	protected:
		rectangle getParentViewport();

	private:
		rectangle		screen_size;
	};
}
#endif	/* __WIESEL_GRAPH_SCENE_H__ */

