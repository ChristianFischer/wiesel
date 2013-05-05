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

#include <wiesel/wiesel-core.def>

#include "platform.h"
#include "application.h"
#include "engine_interfaces.h"

#include <string>


namespace wiesel {

	// some external classes

	class DataSource;
	class FileSystem;
	class IRunnable;
	class Thread;
	class TouchHandler;



	/**
	 * @brief An interface to the game engine.
	 */
	class WIESEL_CORE_EXPORT Engine {
	private:
		Engine();
		virtual ~Engine();

	// getter
	public:
		/**
		 * @brief get the currently active Engine instance.
		 */
		inline static Engine *getInstance() {
			return &instance;
		}

		/**
		 * @brief get the current platform, this engine is running on.
		 */
		inline const std::vector<Platform*> *getPlatforms() {
			return &platforms;
		}

		/**
		 * @brief get the current running application or \c NULL, if none
		 */
		inline Application *getApplication() {
			return application;
		}

		/**
		 * @brief get the system's root file system where all data is stored.
		 * The root filesystem may be read-only or empty for the application.
		 */
		FileSystem *getRootFileSystem();

		/**
		 * @brief get the file system, which stores the assets of the current application.
		 */
		FileSystem *getAssetFileSystem();

		/**
		 * @brief get the file system, where persistent application data can be stored.
		 * The normal data file system should be used for small data like profiles
		 * or savegames. For large data try \ref getExternalDataFileSystem().
		 * @param subdir	This parameter is used to create an application specific
		 *					subdirectory in the user's home directory.
		 *					"subdir" should be an unique name to avoid conflicts with other
		 *					applications. On platforms, which have a dedicated appdata
		 *					location per application, this parameter will be ignored.
		 */
		FileSystem *getDataFileSystem(const std::string &subdir);

		/**
		 * @brief another file system, where persistent application data can be stored.
		 * This may be on another physical drive, which is appropriate for large data.
		 * On some platforms, this may be identical with \ef getDataFileSystem().
		 * @param subdir	This parameter is used to create an application specific
		 *					subdirectory in the user's home directory.
		 *					"subdir" should be an unique name to avoid conflicts with other
		 *					applications. On platforms, which have a dedicated appdata
		 *					location per application, this parameter will be ignored.
		 */
		FileSystem *getExternalDataFileSystem(const std::string &subdir);

	// lifecycle
	public:
		/**
		 * @brief initialize the engine.
		 */
		bool initialize(int argc, char* argv[]);

		/**
		 * @brief terminate the currently registered engine.
		 * Also terminates the currently runnung \ref Application instance.
		 */
		bool shutdown();

		/**
		 * @brief starts the main loop with all existing applications.
		 */
		void run();

		/**
		 * @brief starts the main loop with a specific application class.
		 */
		void run(Application *app);

		/**
		 * @brief requests to stop the main loop after the current frame.
		 */
		void requestExit();

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

		/**
		 * @brief Registers a task, which will be executed on the main thread.
		 * The task will be exewcuted only once and released after that.
		 */
		void runOnMainThread(IRunnable *runnable);

	// static members
	private:
		static Engine					instance;
	
	// instance members
	private:
		ref<Thread>						mainthread;

		std::vector<IRunnable*>			run_once;
		std::vector<IUpdateable*>		updateables;

		bool							exit_requested;

		std::vector<Platform*>			platforms;
		Application*					application;
	};

}

#endif /* __WIESEL_ENGINE_H__ */
