/*
 * filesystem.h
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

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
#endif /* FILESYSTEM_H_ */
