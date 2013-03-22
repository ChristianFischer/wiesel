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
#ifndef __WIESEL_IO_DIRECTORY_H__
#define __WIESEL_IO_DIRECTORY_H__

#include <wiesel/wiesel-core.def>

#include "wiesel/util/shared_object.h"
#include "file.h"

#include <string>
#include <vector>
#include <list>


namespace wiesel {

	class FileSystem;
	class Directory;


	/// Alias type for directory lists.
	typedef std::list<Directory*>	DirectoryList;


	/**
	 * @brief A class representing a directory within a \ref FileSystem.
	 */
	class WIESEL_CORE_EXPORT Directory : public virtual SharedObject
	{
	private:
		Directory() {}

	protected:
		Directory(FileSystem *fs, Directory *parent);

	public:
		virtual ~Directory();

	// getters
	public:
		/**
		 * @brief Get the name of this directory.
		 * The root directory's name may be empty.
		 */
		virtual std::string getName() const = 0;

		/**
		 * @brief Get the full path of this directory.
		 */
		virtual std::string getFullPath() const;

		/**
		 * @brief get the directory's full path on the platform's native file system.
		 * If there's no physical file system, for example on a in-memory stored
		 * file system, this function will return an empty string.
		 * @return the native path or an empty string, if there is no real filesystem.
		 */
		virtual std::string getNativePath() const;

		/**
		 * @brief get the \ref FileSystem, this directory is in.
		 */
		inline FileSystem* getFileSystem() const {
			return fs;
		}

		/**
		 * @brief get the parent-directory of this directory.
		 * The root-directory will return \c NULL.
		 */
		inline Directory* getParent() const {
			return parent;
		}

	// directory content access
	public:
		/**
		 * @brief Get a list of all sub-directories.
		 * Each time calling this function, the folder will be scanned for subdirectories.
		 * There is no caching mechanism.
		 */
		virtual DirectoryList getSubDirectories() = 0;

		/**
		 * @brief Get a list of all files in this directory.
		 * Each time calling this function, the folder will be scanned for subdirectories.
		 * There is no caching mechanism.
		 */
		virtual FileList getFiles() = 0;

		/**
		 * @brief Get a direct subdirectory by it's name.
		 * This function does not resolve relative path names or does a recursive search into other subdirectories.
		 * @returns \c NULL, if there's no direct subdirectory with the given name.
		 */
		virtual Directory *getSubDirectory(const std::string &name);

		/**
		 * @brief Tries to find a specific directory relative to the current directory by it's full name.
		 * When the directory is not found, or the object which was found is a file-object,
		 * \c findDirectory will return \c NULL.
		 * To ensure platform compatibility, directories should be seperated by '/' characters, even on windows.
		 */
		virtual Directory *findDirectory(const std::string &name);

		/**
		 * @brief Tries to find a specific file relative to the current directory by it's full name.
		 * When the file is not found, or the object which was found is a directory,
		 * \c findFile will return \c NULL.
		 * To ensure platform compatibility, directories should be seperated by '/' characters, even on windows.
		 */
		virtual File *findFile(const std::string &name);

		/**
		 * @brief Tries to create a directory relative to the current one.
		 * It's possible to create multiple subdirectories at once.
		 * When successful, the new directory will be returned. If the directory
		 * already exists, the existing directory will be returned.
		 * On failure, \c NULL will be returned.
		 */
		virtual Directory *createDirectory(const std::string &name);

		/**
		 * @brief Tries to create a file relative to the current directory.
		 * When successful, the new file will be returned. If the file
		 * already exists, the existing file object will be returned.
		 * When the file's directory does not exist, it will NOT be created.
		 * On failure, \c NULL will be returned.
		 */
		virtual File *createFile(const std::string &name);

		/**
		 * @brief Checks if we have read-access to this directory.
		 * Without read-access it's not possible to get a listing of this directory.
		 */
		virtual bool canRead() const = 0;

		/**
		 * @breif Checks if we have write-access to this directory.
		 * Without write-access we cannot create files or subdirectories here.
		 */
		virtual bool canWrite() const = 0;

	// to be implemented by subclasses
	protected:
		/**
		 * @brief Try to create a specific directory.
		 */
		virtual Directory *doCreateDirectory(const std::string &name) = 0;

		/**
		 * @brief Try to create a specific file.
		 */
		virtual File *doCreateFile(const std::string &name) = 0;

	// sort utilities
	public:
		/**
		 * @brief Sort a list of directories by their names.
		 */
		static void sortByName(DirectoryList &list, bool asc=true);

	private:
		FileSystem*	fs;
		Directory*	parent;
	};



	/**
	 * @brief Predicate function to be used for sorting lists of directories by their names.
	 */
	inline bool DirectorySortByNameAscPredicate(const Directory *lhs, const Directory *rhs) {
		return lhs->getName() < rhs->getName();
	}


	/**
	 * @brief Predicate function to be used for sorting lists of directories by their names.
	 */
	inline bool DirectorySortByNameDescPredicate(const Directory *lhs, const Directory *rhs) {
		return lhs->getName() > rhs->getName();
	}

} /* namespace wiesel */
#endif /* __WIESEL_IO_DIRECTORY_H__ */
