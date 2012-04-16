/*
 * generic_root_fs.h
 *
 *  Created on: 10.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_PLATFORM_ANDROID_FILESUPPORT_ANDROID_ASSET_FS_H__
#define __WIESEL_PLATFORM_ANDROID_FILESUPPORT_ANDROID_ASSET_FS_H__

#include "wiesel/io/filesystem.h"

#include <android/asset_manager.h>


namespace wiesel {

	class AndroidAssetFileSystemDirectory;



	class AndroidAssetFileSystem : public FileSystem
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



	class AndroidAssetFileSystemDirectory : public Directory
	{
	public:
		AndroidAssetFileSystemDirectory(AndroidAssetFileSystem *fs, Directory *parent, const std::string &name);
		~AndroidAssetFileSystemDirectory();

		virtual std::string getName() const;
		virtual Directory *getSubDirectory(const std::string &name);
		virtual DirectoryList getSubDirectories();
		virtual FileList getFiles();

		inline AAssetManager *getAssetManager() {
			return ((AndroidAssetFileSystem*)getFileSystem())->getAssetManager();
		}

	private:
		std::string		name;
	};



	class AndroidAssetFileSystemFile : public File
	{
	public:
		AndroidAssetFileSystemFile(AndroidAssetFileSystemDirectory *parent, const std::string &name);
		~AndroidAssetFileSystemFile();

		virtual std::string getName() const;

		virtual DataBuffer *getContent();

		inline AAssetManager *getAssetManager() {
			return ((AndroidAssetFileSystem*)getParent()->getFileSystem())->getAssetManager();
		}

	private:
		std::string		name;
	};

} /* namespace wiesel */
#endif /* __WIESEL_PLATFORM_ANDROID_FILESUPPORT_ANDROID_ASSET_FS_H__ */
