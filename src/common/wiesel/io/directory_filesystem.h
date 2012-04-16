/*
 * directory_filesystem.h
 *
 *  Created on: 16.04.2012
 *      Author: Baldur
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
		virtual DataBuffer *getContent();

	private:
		File	*enclosed_file;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_DIRECTORY_FILESYSTEM_H__ */
