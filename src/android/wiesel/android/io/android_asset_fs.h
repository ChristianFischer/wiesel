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
#ifndef __WIESEL_ANDROID_ASSET_FS_H__
#define __WIESEL_ANDROID_ASSET_FS_H__

#include <wiesel/wiesel-core.def>

#include "wiesel/io/filesystem.h"

#include <android/asset_manager.h>


namespace wiesel {
namespace android {

	class AndroidAssetFileSystemDirectory;



	class WIESEL_CORE_EXPORT AndroidAssetFileSystem : public FileSystem
	{
	private:
		AndroidAssetFileSystem() {};

	public:
		AndroidAssetFileSystem(AAssetManager* assetManager);
		virtual ~AndroidAssetFileSystem();

		virtual Directory *getRootDirectory();

		inline AAssetManager *getAssetManager() {
			return assetManager;
		}

	private:
		AndroidAssetFileSystemDirectory*	root;
		AAssetManager*						assetManager;
	};



	class WIESEL_CORE_EXPORT AndroidAssetFileSystemDirectory : public Directory
	{
	public:
		AndroidAssetFileSystemDirectory(AndroidAssetFileSystem *fs, Directory *parent, const std::string &name);
		~AndroidAssetFileSystemDirectory();

		virtual std::string getName() const;
		virtual Directory *getSubDirectory(const std::string &name);
		virtual DirectoryList getSubDirectories();
		virtual FileList getFiles();

		virtual bool canRead() const;
		virtual bool canWrite() const;

		inline AAssetManager *getAssetManager() {
			return ((AndroidAssetFileSystem*)getFileSystem())->getAssetManager();
		}

	protected:
		virtual Directory *doCreateDirectory(const std::string &name);
		virtual File *doCreateFile(const std::string &name);

	private:
		std::string		name;
	};



	class WIESEL_CORE_EXPORT AndroidAssetFileSystemFile : public File
	{
	public:
		AndroidAssetFileSystemFile(AndroidAssetFileSystemDirectory *parent, const std::string &name);
		~AndroidAssetFileSystemFile();

		virtual std::string getName() const;

		virtual DataBuffer *loadContent();

		virtual bool canRead() const;
		virtual bool canWrite() const;

		inline AAssetManager *getAssetManager() {
			return ((AndroidAssetFileSystem*)getParent()->getFileSystem())->getAssetManager();
		}

	private:
		std::string		name;
	};

} /* namespace android */
} /* namespace wiesel */
#endif /* __WIESEL_ANDROID_ASSET_FS_H__ */
