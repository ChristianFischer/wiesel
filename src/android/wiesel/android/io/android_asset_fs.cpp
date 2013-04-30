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

#ifdef __ANDROID__

#include "android_asset_fs.h"
#include "wiesel/io/databuffer.h"
#include "wiesel/util/log.h"
#include <dirent.h>


using namespace wiesel;
using namespace wiesel::android;
using namespace std;


AndroidAssetFileSystem::AndroidAssetFileSystem(AAssetManager *assetManager) {
	this->assetManager = assetManager;
	root = keep(new AndroidAssetFileSystemDirectory(this, NULL, ""));
	return;
}

AndroidAssetFileSystem::~AndroidAssetFileSystem() {
	safe_release(root);
	return;
}


Directory *AndroidAssetFileSystem::getRootDirectory() {
	return root;
}








AndroidAssetFileSystemDirectory::AndroidAssetFileSystemDirectory(AndroidAssetFileSystem *fs, Directory *parent, const string &name)
:	Directory(fs, parent), name(name)
{
	return;
}

AndroidAssetFileSystemDirectory::~AndroidAssetFileSystemDirectory() {
	return;
}


string AndroidAssetFileSystemDirectory::getName() const {
	return name;
}


Directory *AndroidAssetFileSystemDirectory::getSubDirectory(const string &name) {
	// get the full path
	string path = (getFullPath() + "/" + name);

	// strip leading slash
	if (path.size() >= 1 && path[0] == '/') {
		path.erase(path.begin());
	}

	// check, if this directory really exists
	AAssetDir *dir = AAssetManager_openDir(getAssetManager(), path.c_str());
	if (dir != NULL) {
		AndroidAssetFileSystem *fs = dynamic_cast<AndroidAssetFileSystem*>(getFileSystem());
		AndroidAssetFileSystemDirectory *subdir = new AndroidAssetFileSystemDirectory(fs, this, name);
		AAssetDir_close(dir);

		return subdir;
	}

	return NULL;
}


DirectoryList AndroidAssetFileSystemDirectory::getSubDirectories() {
	DirectoryList directories;

	// it's not possible to get a directory listing from the asset filesystem,
	// so, this function will always return a empty list.

	return directories;
}


FileList AndroidAssetFileSystemDirectory::getFiles() {
	FileList files;
	AAssetDir *dir;
	const char *filename;

	// strip leading slash
	string path = getFullPath();
	if (path.size() >= 1 && path[0] == '/') {
		path.erase(path.begin());
	}

	if ((dir = AAssetManager_openDir(getAssetManager(), path.c_str())) != NULL) {
		while ((filename = AAssetDir_getNextFileName(dir)) != NULL) {
			files.push_back(new AndroidAssetFileSystemFile(this, filename));
		}

		AAssetDir_close(dir);
	}

	return files;
}


bool AndroidAssetFileSystemDirectory::canRead() const {
	return true;
}


bool AndroidAssetFileSystemDirectory::canWrite() const {
	return false;
}


Directory *AndroidAssetFileSystemDirectory::doCreateDirectory(const string &name) {
	return NULL;
}


File *AndroidAssetFileSystemDirectory::doCreateFile(const string &name) {
	return NULL;
}







AndroidAssetFileSystemFile::AndroidAssetFileSystemFile(AndroidAssetFileSystemDirectory *parent, const string &name)
: File(parent), name(name)
{
	return;
}

AndroidAssetFileSystemFile::~AndroidAssetFileSystemFile() {
	return;
}


string AndroidAssetFileSystemFile::getName() const {
	return name;
}


DataBuffer *AndroidAssetFileSystemFile::loadContent() {
	string path = getFullPath();

	// strip leading slash
	if (path[0] == '/') {
		path.erase(path.begin());
	}

	// open the asset object
	AAsset *asset = AAssetManager_open(getAssetManager(), path.c_str(), AASSET_MODE_BUFFER);

	// get the asset's data
	if (asset) {
		const void *data   = AAsset_getBuffer(asset);
		off_t       size   = AAsset_getLength(asset);
		DataBuffer *buffer = NULL;

		if (data && size) {
			buffer = ExclusiveDataBuffer::createCopyOf(reinterpret_cast<DataBuffer::data_t>(data), size);
		}

		// close the buffer
		AAsset_close(asset);

		return buffer;
	}

	return NULL;
}


bool AndroidAssetFileSystemFile::canRead() const {
	return true;
}


bool AndroidAssetFileSystemFile::canWrite() const {
	return false;
}


#endif // __ANDROID__
