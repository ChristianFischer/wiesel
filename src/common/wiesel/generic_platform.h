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
#ifndef __WIESEL_GENERIC_PLATFORM_H__
#define __WIESEL_GENERIC_PLATFORM_H__

#include <wiesel/wiesel-common.def>

#include <wiesel/platform.h>

#include <map>


namespace wiesel {

	class GenericFileSystem;


	/**
	 * @brief A generic platform implementation which should fit
	 * most windows and unix like implementations.
	 */
	class WIESEL_COMMON_EXPORT GenericPlatform : public Platform
	{
	public:
		GenericPlatform();
		virtual ~GenericPlatform();

	// overridables
	protected:
		virtual bool onInit();
		virtual void onShutdown();

		virtual void onRunFirst();
		virtual bool onRun();

	// utilities
	public:
		/**
		 * @brief Get the value of the environment variable with the given name.
		 * @param name The name of the requested environment variable,
		 * @return The value of the requested environment variable,
		 */
		std::string getEnv(const std::string &name) const;

		/**
		 * @brief Get the current working directory name.
		 */
		std::string getWorkingDirName() const;

		/**
		 * @brief Get the name of the current user's home directory.
		 */
		std::string getHomeDirName() const;

		/**
		 * @brief Get the current users application data directory name.
		 */
		std::string getAppDataDirName() const;

		/**
		 * @brief Converts a path into the format used by the engine.
		 */
		static void convertPathToEngineStyle(std::string *path);

	public:
		virtual FileSystem *getRootFileSystem();
		virtual FileSystem *getAssetFileSystem();
		virtual FileSystem *getDataFileSystem(const std::string &subdir);
		virtual FileSystem *getExternalDataFileSystem(const std::string &subdir);

	protected:
		FileSystem*		root_fs;
		FileSystem*		asset_fs;

		std::map<std::string,FileSystem*>	data_fs;
	};

}

#endif /* __WIESEL_GENERIC_PLATFORM_H__ */
