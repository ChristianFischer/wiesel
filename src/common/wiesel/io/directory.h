/*
 * directory.h
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_IO_DIRECTORY_H__
#define __WIESEL_IO_DIRECTORY_H__

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
	class Directory : public SharedObject
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
		virtual std::string getFullPath();

		/**
		 * @brief get the \ref FileSystem, this directory is in.
		 */
		inline FileSystem* getFileSystem() {
			return fs;
		}

		/**
		 * @brief get the parent-directory of this directory.
		 * The root-directory will return \c NULL.
		 */
		inline Directory* getParent() {
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
