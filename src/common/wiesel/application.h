/*
 * application.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
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
