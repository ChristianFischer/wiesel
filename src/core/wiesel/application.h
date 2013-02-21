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
#ifndef __WIESEL_APPLICATION_H__
#define __WIESEL_APPLICATION_H__

#include <wiesel/wiesel-core.def>

#include "wiesel/graph/scene.h"
#include "wiesel/module.h"

#include <vector>


namespace wiesel {


	/**
	 * @brief An abstract class implementing the application logic.
	 */
	class WIESEL_CORE_EXPORT Application : public Module
	{
	public:
		Application();
		virtual ~Application();

		/**
		 * @brief initialize the application before running.
		 */
		virtual bool onInit() = 0;

		/**
		 * @brief release contents before stopping the application.
		 */
		virtual void onShutdown() = 0;

		/**
		 * @brief notification when the application has lost focus and is no longer in the foreground.
		 */
		virtual void onEnterBackground() {}

		/**
		 * @brief notification when the application will enter the foreground and is active.
		 */
		virtual void onEnterForeground() {}

		/**
		 * @brief suspends the application, for example when the user switches to another task.
		 */
		virtual void onSuspend() {}

		/**
		 * @brief resume the application after beeing suspended.
		 */
		virtual void onResumeSuspended() {}

		/**
		 * @brief called each frame to process application data.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRun(float dt);

		/**
		 * @brief called each frame to render graphics.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRender(video::RenderContext *render_context);

	// scene stack
	public:
		/**
		 * @brief Push a new scene to the scene stack.
		 * @return \c true, when the scene was added, \c false otherwise.
		 */
		bool pushScene(Scene *scene);

		/**
		 * @brief Removes a scene from the scene stack.
		 */
		void removeScene(Scene *scene);

		/**
		 * @brief removes all scenes from the scene stack.
		 */
		void clearSceneStack();

		/**
		 * @brief Get the topmost scene in the scene stack.
		 * @return the topmost scene, or \c NULL, when the scene stack is empty.
		 */
		inline Scene *getTopScene();

		/**
		 * @brief Provides access to the scene stack.
		 * @return A const-list of all scenes in the stack, which cannot be manipulated.
		 */
		inline const SceneList *getSceneStack() const {
			return &scene_stack;
		}

	private:
		SceneList		scene_stack;
	};

}

#endif /* __WIESEL_APPLICATION_H__ */
