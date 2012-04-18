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
		virtual void onRun() = 0;

		/**
		 * @brief called each frame to render graphics.
		 * DUMMY - will be replaced by scene graph later.
		 */
		virtual void onRender() = 0;
	};

}

#endif /* __WIESEL_APPLICATION_H__ */
