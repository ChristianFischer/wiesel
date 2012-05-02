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
#ifndef __WIESEL_IO_DIRECTORY_FILESYSTEM_H__
#define __WIESEL_IO_DIRECTORY_FILESYSTEM_H__

#include "wiesel/io/filesystem.h"


namespace wiesel {


	class DirectoryFileSystemRootDirectory;



	/**
	 * @brief A virtual \ref FileSystem, which uses a directory of another filesystem
	 * as it's root directory. Each directory of this filesystem corresponds to a
	 * directory of the enclosed filesystem.
	 */
	class DirectoryFileSystem : public FileSystem
	{
	private:
		DirectoryFileSystem() {};

	public:
		DirectoryFileSystem(Directory *enclosed_root);
		virtual ~DirectoryFileSystem();

		/**
		 * @brief get the enclosed directory of this filesystem's root directory.
		 */
		virtual Directory *getEnclosedRootDirectory();

		/**
		 * @brief get the enclosed directory of this filesystem's root directory.
		 */
		virtual const Directory *getEnclosedRootDirectory() const;

	public:
		virtual Directory *getRootDirectory();

	private:
		Directory							*enclosed_root;
		DirectoryFileSystemRootDirectory	*root;
	};



	/**
	 * @brief A virtual \ref Directory object, which wraps a directory of an other filesystem.
	 */
	class DirectoryFileSystemDirectory : public Directory
	{
	public:
		DirectoryFileSystemDirectory(DirectoryFileSystem *fs, DirectoryFileSystemDirectory *parent, Directory *enclosed_directory);
		~DirectoryFileSystemDirectory();

		/**
		 * @brief get the enclosed directory object of this virtual directory.
		 */
		virtual Directory *getEnclosedDirectory();

		/**
		 * @brief get the enclosed directory object of this virtual directory.
		 */
		virtual const Directory *getEnclosedDirectory() const;

	public:
		virtual std::string getName() const;
		virtual std::string getNativePath();
		virtual DirectoryList getSubDirectories();
		virtual FileList getFiles();

	private:
		Directory	*enclosed_directory;
	};



	/**
	 * @brief A virtual \ref Directory object, which wraps the root directory of a \ref DirectoryFileSystem.
	 */
	class DirectoryFileSystemRootDirectory : public DirectoryFileSystemDirectory
	{
	public:
		DirectoryFileSystemRootDirectory(DirectoryFileSystem *fs, Directory *enclosed_directory);
		~DirectoryFileSystemRootDirectory();

		virtual std::string getName() const;
	};



	/**
	 * @brief A virtual \ref File object, which wraps a directory of an other filesystem.
	 */
	class DirectoryFileSystemFile : public File
	{
	public:
		DirectoryFileSystemFile(DirectoryFileSystemDirectory *parent, File *enclosed_file);
		~DirectoryFileSystemFile();

		/**
		 * @brief get the enclosed file object of this virtual file.
		 */
		virtual File *getEnclosedFile();

		/**
		 * @brief get the enclosed file object of this virtual file.
		 */
		virtual const File *getEnclosedFile() const;

	public:
		virtual std::string getName() const;
		virtual std::string getNativePath();
		virtual DataBuffer *getContent();

	private:
		File	*enclosed_file;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_DIRECTORY_FILESYSTEM_H__ */
