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

#include <string>


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

	// stuff
	public:
		/**
		 * @brief decodes an image from file into a buffer.
		 * Takes care of power-of-two size.
		 * @param filename		Name of the file to load.
		 * @param buffer		A buffer to receive the image data.
		 * @param size			A pointer to receive the size of the buffer.
		 * @param width			A pointer to receive the dimension of the image.
		 * @param height		A pointer to receive the dimension of the image.
		 * @param *_bits		Points to store the bit size of each RGBA component.
		 * 						If non-zero, the implementation should try to force the bit-size to the given value.
		 * @param as_textzre	When \c true, the implementation may try to fit the texture into required parameters.
		 */
		virtual bool decodeImage(
								const std::string &filename,
								unsigned char **pBuffer, size_t *pSize, unsigned int *pWidth, unsigned int *pHeight,
								int *pRbits, int *pGbits, int *pBbits, int *pAbits,
								bool as_texture
		) = 0;

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
