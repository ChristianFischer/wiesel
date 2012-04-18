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
#ifndef __WIESEL_IO_FILESYSTEM_H__
#define __WIESEL_IO_FILESYSTEM_H__

#include "directory.h"
#include "file.h"

#include <string>
#include <vector>
#include <list>


namespace wiesel {

	/**
	 * @brief A class representing a filesystem to provide access to files and directories of a platform.
	 */
	class FileSystem
	{
	protected:
		FileSystem();

	public:
		virtual ~FileSystem();

		/**
		 * @brief Get the root-directory of the current file system.
		 * On unix-based platforms, this is the root-directory "/".
		 * On windows-based platforms, this would be a virtual folder containing "C:", "D:", ... subdirectories.
		 */
		virtual Directory *getRootDirectory() = 0;

		/**
		 * @brief Tries to find a specific directory by it's full name.
		 * When the directory is not found, or the object which was found is a file-object,
		 * \c findDirectory will return \c NULL.
		 * To ensure platform compatibility, directories should be seperated by '/' characters, even on windows.
		 */
		virtual Directory *findDirectory(const std::string &name);

		/**
		 * @brief Tries to find a specific file by it's full name.
		 * When the file is not found, or the object which was found is a directory,
		 * \c findFile will return \c NULL.
		 * To ensure platform compatibility, directories should be seperated by '/' characters, even on windows.
		 */
		virtual File *findFile(const std::string &name);
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_FILESYSTEM_H__ */
