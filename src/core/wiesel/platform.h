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
#ifndef __WIESEL_PLATFORM_H__
#define __WIESEL_PLATFORM_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/module.h>

#include <string>


namespace wiesel {

	// some external classes

	class FileSystem;
	class Engine;


	/**
	 * @brief An interface to the current platform.
	 */
	class WIESEL_CORE_EXPORT Platform : public Module
	{
	friend class Engine;

	protected:
		Platform();
		virtual ~Platform();

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
		virtual FileSystem *getDataFileSystem(const std::string &subdir) = 0;

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
		virtual FileSystem *getExternalDataFileSystem(const std::string &subdir) = 0;
	};

}

#endif /* __WIESEL_ENGINE_H__ */
