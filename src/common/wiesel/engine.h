/*
 * engine.h
 *
 *  Created on: 19.03.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_ENGINE_H__
#define __WIESEL_ENGINE_H__

#include "screen.h"
#include "application.h"


namespace wiesel {

	/**
	 * @brief An interface to the game engine.
	 */
	class Engine {
	protected:
		Engine();
		virtual ~Engine();

	public:
		/**
		 * @brief get the currently active Engine instance.
		 */
		static Engine *getCurrent() {
			return current_instance;
		}

		/**
		 * @brief get the current running application or \c NULL, if none
		 */
		static Application *getApplication() {
			return current_app;
		}

		/**
		 * @brief Install a new Engine isntance, if currently no other instance is active.
		 * @returns \c true, if the engine was installed successfully.
		 * @returns \c false, if the installation process failed or another engine is already running.
		 */
		static bool install(Engine *engine);

		/**
		 * @brief terminate the currently registered engine.
		 * Also terminates the currently runnung \ref Application instance.
		 */
		static bool shutdown();

		/**
		 * @brief starts the main loop with the currently installed engine.
		 */
		static void run(Application *application);

		/**
		 * @brief requests to stop the main loop after the current frame.
		 */
		static void requestExit();

	// public methods
	public:
		/**
		 * @brief checks, if the current instance is still the active \ref Engine instance.
		 */
		inline bool isActive() const {
			return (this == current_instance);
		}

		/**
		 * @brief get the screen object.
		 */
		inline Screen *getScreen() {
			return screen;
		}

	// overridables
	protected:
		/**
		 * @brief initialize the engine on installation process.
		 */
		virtual bool onInit() = 0;

		/**
		 * @brief clean up on shutdown.
		 * The engine should release all resources here.
		 */
		virtual void onShutdown() = 0;

		/**
		 * @brief first update of the engine.
		 * May be used to invoke \ref startApp, but can be left empty.
		 */
		virtual void onRunFirst() = 0;

		/**
		 * @brief called periodically by the main loop to process the message loop.
		 * @return \c true to stop the main loop
		 */
		virtual bool onRun() = 0;

	// application control
	protected:
		/**
		 * @brief start the application, when application state is \ref Application_Uninitialized.
		 * Fails, if no application is available.
		 * In other states, this method will have no effect.
		 */
		static void startApp();

		/**
		 * @brief suspends a running application.
		 * Fails, if no application is available.
		 * In other states, this method will have no effect.
		 */
		static void suspendApp();

		/**
		 * @brief resumes a suspended application.
		 * Fails, if no application is available.
		 * In other states, this method will have no effect.
		 */
		static void resumeSuspendedApp();

	// static members
	private:
		static Engine*			current_instance;
		static Application*		current_app;
		static ApplicationState	current_app_state;
		static bool				exit_requested;

	// instance members
	protected:
		Screen*		screen;
	};

}

#endif /* __WIESEL_ENGINE_H__ */
