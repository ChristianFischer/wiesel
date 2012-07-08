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
#ifndef __WIESEL_ENGINE_H__
#define __WIESEL_ENGINE_H__

#include "screen.h"
#include "application.h"
#include "engine_interfaces.h"

#include <string>


namespace wiesel {

	// some external classes

	class DataSource;
	class FileSystem;
	class TouchHandler;



	/**
	 * @brief manages the current state of the application.
	 */
	enum EngineState
	{
		Engine_Uninitialized,		//<! The initial state of the engine before initialization.
		Engine_Running,				//<! The engine was initialized and is running.
		Engine_Background,			//<! The engine is running in background
		Engine_Suspended,			//<! The engine was suspended - no update or render calls will be invoked.
	};



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

	// register/remove objects
	public:
		/**
		 * @brief Registers an IUpdateable object to receive periodically updates.
		 */
		void registerUpdateable(IUpdateable *updateable);

		/**
		 * @brief Removes an IUpdateable object, so it will no longer receive updates.
		 */
		void unregisterUpdateable(IUpdateable *updateable);

	// public methods
	public:
		/**
		 * @brief get the engine's current state.
		 */
		inline EngineState getState() const {
			return state;
		}

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
		 * @brief the application will enter the background.
		 */
		static void enterBackground();

		/**
		 * @brief the application will enter the foreground.
		 */
		static void enterForeground();

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
		 * After resuming, the application is in the Background state.
		 */
		static void resumeSuspendedApp();

	// filesystems
	public:
		/**
		 * @brief get the system's root file system where all data is stored.
		 * The root filesystem may be read-only or empty for the application.
		 */
		virtual FileSystem *getRootFileSystem() = 0;

		/**
		 * @brief get the file system, which stores the assets of the current application.
		 */
		virtual FileSystem *getAssetFileSystem() = 0;

	// additional objects
	public:
		virtual TouchHandler *getTouchHandler() = 0;

	// stuff
	public:
		/**
		 * @brief decodes an image from file into a buffer.
		 * Takes care of power-of-two size.
		 * @param data				A \ref DataSource, the image data will be loaded from.
		 * @param pBuffer			A buffer to receive the image data.
		 * @param pSize				A pointer to receive the size of the buffer.
		 * @param pWidth			A pointer to receive the dimension of the image.
		 * @param pHeight			A pointer to receive the dimension of the image.
		 * @param pOriginalWidth	A pointer to receive the original dimension of the image, if it was resized to match pot-size.
		 * @param pOriginalHeight	A pointer to receive the original dimension of the image, if it was resized to match pot-size.
		 * @param p*Bits			Points to store the bit size of each RGBA component.
		 * 							If non-zero, the implementation should try to force the bit-size to the given value.
		 * @param as_textzre		When \c true, the implementation may try to fit the texture into required parameters.
		 */
		virtual bool decodeImage(
								DataSource *data,
								unsigned char **pBuffer, size_t *pSize,
								unsigned int *pWidth, unsigned int *pHeight,
								unsigned int *pOriginalWidth, unsigned int *pOriginalHeight,
								int *pRbits, int *pGbits, int *pBbits, int *pAbits,
								bool as_texture
		) = 0;

	// static members
	private:
		static Engine*			current_instance;
		static Application*		current_app;
		static bool				exit_requested;

	// instance members
	protected:
		Screen*		screen;

	private:
		std::vector<IUpdateable*>		updateables;

		EngineState						state;
	};

}

#endif /* __WIESEL_ENGINE_H__ */
