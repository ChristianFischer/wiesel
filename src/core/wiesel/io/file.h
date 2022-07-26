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
#ifndef __WIESEL_IO_FILE_H__
#define __WIESEL_IO_FILE_H__

#include <wiesel/wiesel-core.def>

#include "wiesel/util/shared_object.h"
#include "datasource.h"

#include <string>
#include <vector>
#include <list>


namespace wiesel {

	class FileSystem;
	class Directory;
	class File;


	/// Alias type for file lists.
	typedef wiesel::ref<File>::list		FileList;


	/**
	 * @brief A class representing a file within a \ref FileSystem.
	 */
	class WIESEL_CORE_EXPORT File : public virtual SharedObject
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
		virtual std::string getFullPath() const;
		
		/**
		 * @brief get the file's full path on the platform's native file system.
		 * If there's no physical file system, for example on a in-memory stored
		 * file system, this function will return an empty string.
		 * @return the native path or an empty string, if there is no real filesystem.
		 */
		virtual std::string getNativePath() const;

		/**
		 * @brief get the file's extension.
		 * For example: "txt" (no loading dot)
		 */
		virtual std::string getExtension() const;

		/**
		 * @brief Get a \ref DataSource referencing to this file.
		 * Usually this would be a \ref FileDataSource, but that's not guaranteed.
		 */
		virtual DataSource *asDataSource();

		/**
		 * @brief loads a \ref DataBuffer with the content of this file.
		 */
		virtual DataBuffer *loadContent() = 0;

		/**
		 * @brief load the full file content as string-object.
		 * Binary files may return an empty string or garbage.
		 * When the file couldn't be opened, an empty string will be returned.
		 * To handle the case, a file was not readable, use \ref getContent().
		 */
		virtual std::string loadContentAsString();

		/**
		 * @brief read a text file's content as a vector of lines.
		 * Binary files may return an empty list or garbage.
		 */
		virtual std::vector<std::string> loadLines();

		/**
		 * @brief get the parent-directory of this file.
		 */
		inline Directory* getParent() const {
			return parent;
		}

		/**
		 * @brief Checks if we have can read data from this file.
		 * Without read-access, all methods to read from this file will fail.
		 */
		virtual bool canRead() const = 0;

		/**
		 * @breif Checks if we can write data into this file.
		 */
		virtual bool canWrite() const = 0;

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




	/**
	 * @brief A \ref DataSource object which provides access to a \ref FileObject.
	 */
	class WIESEL_CORE_EXPORT FileDataSource : public DataSource
	{
	private:
		FileDataSource() : file(NULL) {};

	public:
		/**
		 * @brief Create a new \ref FileDataSource.
		 */
		FileDataSource(File *file);

		~FileDataSource();

		virtual DataBuffer *getDataBuffer();
		
		virtual void releaseDataBuffer();

		/**
		 * @brief Get the \ref File object associated with this file buffer.
		 */
		virtual File *getFile();

	private:
		File* 			file;
		DataBuffer*		content;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_FILE_H__ */
