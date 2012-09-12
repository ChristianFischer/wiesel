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
#ifndef __WIESEL_PLATFORM_GENERIC_FILESUPPORT_GENERIC_ROOT_FS_H__
#define __WIESEL_PLATFORM_GENERIC_FILESUPPORT_GENERIC_ROOT_FS_H__

#include <wiesel/wiesel-common.def>

#include "wiesel/io/filesystem.h"


namespace wiesel {

	class GenericFileSystemDirectory;



	class WIESEL_COMMON_EXPORT GenericFileSystem : public FileSystem
	{
	public:
		GenericFileSystem();
		virtual ~GenericFileSystem();

		virtual Directory *getRootDirectory();

	private:
		GenericFileSystemDirectory *root;
	};



	class WIESEL_COMMON_EXPORT GenericFileSystemDirectory : public Directory
	{
	public:
		GenericFileSystemDirectory(GenericFileSystem *fs, Directory *parent, const std::string &name);
		~GenericFileSystemDirectory();

		virtual std::string getName() const;
		virtual std::string getFullPath();
		virtual std::string getNativePath();
		virtual DirectoryList getSubDirectories();
		virtual FileList getFiles();

	private:
		std::string		name;
	};



	class WIESEL_COMMON_EXPORT GenericFileSystemFile : public File
	{
	public:
		GenericFileSystemFile(GenericFileSystemDirectory *parent, const std::string &name);
		~GenericFileSystemFile();

		virtual std::string getName() const;
		virtual std::string getNativePath();

		virtual DataBuffer *getContent();

	private:
		std::string		name;
	};

} /* namespace wiesel */
#endif /* __WIESEL_PLATFORM_GENERIC_FILESUPPORT_GENERIC_ROOT_FS_H__ */
