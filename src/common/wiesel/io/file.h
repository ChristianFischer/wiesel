/*
 * file.h
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_IO_FILE_H__
#define __WIESEL_IO_FILE_H__

#include "wiesel/util/shared_object.h"

#include <string>
#include <vector>
#include <list>


namespace wiesel {

	class FileSystem;
	class Directory;
	class File;


	/// Alias type for file lists.
	typedef std::list<File*>	FileList;


	/**
	 * @brief A class representing a file within a \ref FileSystem.
	 */
	class File : public SharedObject
	{
	private:
		File() {}

	protected:
		File(Directory *parent);

	public:
		virtual ~File();

	// getters
	public:
		/**
		 * @brief get the file's name.
		 * For example: "myfile.txt"
		 */
		virtual std::string getName() const = 0;

		/**
		 * @brief get the file's full path including it's name.
		 * For example: "/home/user/myfile.txt"
		 * To get the path only, try getParent()->getFullPath()
		 */
		virtual std::string getFullPath();

		/**
		 * @brief get the file's extension.
		 * For example: "txt" (no loading dot)
		 */
		virtual std::string getExtension() const;

		/**
		 * @brief get the full file content as string-object.
		 * Binary files may return an empty string or garbage.
		 */
		virtual std::string getContentAsString() const;

		/**
		 * @brief read a text file's content as a vector of lines.
		 * Binary files may return an empty list or garbage.
		 */
		virtual std::vector<std::string> getLines() const;

		/**
		 * @brief get the parent-directory of this file.
		 */
		inline Directory* getParent() {
			return parent;
		}

	public:
		/**
		 * @brief Sort a list of files by their names.
		 */
		static void sortByName(FileList &list, bool asc=true);

	private:
		Directory*	parent;
	};



	/**
	 * @brief Predicate function to be used for sorting lists of files by their names.
	 */
	inline bool FileSortByNameAscPredicate(const File *lhs, const File *rhs) {
		return lhs->getName() < rhs->getName();
	}


	/**
	 * @brief Predicate function to be used for sorting lists of files by their names.
	 */
	inline bool FileSortByNameDescPredicate(const File *lhs, const File *rhs) {
		return lhs->getName() > rhs->getName();
	}

} /* namespace wiesel */
#endif /* __WIESEL_IO_FILE_H__ */
