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

#include "graph/scene.h"
#include <vector>


namespace wiesel {


	/**
	 * @brief manages the current state of the application.
	 */
	enum ApplicationState
	{
		Application_Uninitialized,		//<! The initial state of the application before initialization.
		Application_Running,			//<! The application was initialized and is running.
		Application_Suspended,			//<! The application was suspended - no update or render calls will be invoked.
	};


	/**
	 * @brief An abstract class implementing the application logic.
	 */
	class Application {
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
		virtual void onRun();

		/**
		 * @brief called each frame to render graphics.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRender();

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
