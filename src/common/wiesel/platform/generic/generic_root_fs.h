/*
 * generic_root_fs.h
 *
 *  Created on: 10.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_PLATFORM_GENERIC_FILESUPPORT_ANDROID_ROOT_FS_H__
#define __WIESEL_PLATFORM_GENERIC_FILESUPPORT_ANDROID_ROOT_FS_H__

#include "wiesel/io/filesystem.h"


namespace wiesel {

	class GenericFileSystemDirectory;



	class GenericFileSystem : public FileSystem
	{
	public:
		GenericFileSystem();
		virtual ~GenericFileSystem();

		virtual Directory *getRootDirectory();

	private:
		GenericFileSystemDirectory *root;
	};



	class GenericFileSystemDirectory : public Directory
	{
	public:
		GenericFileSystemDirectory(GenericFileSystem *fs, Directory *parent, const std::string &name);
		~GenericFileSystemDirectory();

		virtual std::string getName() const;
		virtual DirectoryList getSubDirectories();
		virtual FileList getFiles();

	private:
		std::string		name;
	};



	class GenericFileSystemFile : public File
	{
	public:
		GenericFileSystemFile(GenericFileSystemDirectory *parent, const std::string &name);
		~GenericFileSystemFile();

		virtual std::string getName() const;

	private:
		std::string		name;
	};

} /* namespace wiesel */
#endif /* __WIESEL_PLATFORM_GENERIC_FILESUPPORT_ANDROID_ROOT_FS_H__ */
