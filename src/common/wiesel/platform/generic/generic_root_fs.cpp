/*
 * generic_root_fs.cpp
 *
 *  Created on: 10.04.2012
 *      Author: Baldur
 */

#include "generic_root_fs.h"
#include "wiesel/util/log.h"
#include <dirent.h>


using namespace wiesel;
using namespace std;


GenericFileSystem::GenericFileSystem() {
	root = new GenericFileSystemDirectory(this, NULL, "");
	root->retain();
	return;
}

GenericFileSystem::~GenericFileSystem() {
	root->release();
	return;
}


Directory *GenericFileSystem::getRootDirectory() {
	return root;
}








GenericFileSystemDirectory::GenericFileSystemDirectory(GenericFileSystem *fs, Directory *parent, const string &name)
:	Directory(fs, parent), name(name)
{
	return;
}

GenericFileSystemDirectory::~GenericFileSystemDirectory() {
	return;
}


string GenericFileSystemDirectory::getName() const {
	return name;
}


DirectoryList GenericFileSystemDirectory::getSubDirectories() {
	DirectoryList directories;
	struct dirent *dirp;
	DIR *dp;

	if ((dp = opendir((getFullPath() + "/").c_str())) != NULL) {
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type != DT_DIR) {
				continue;
			}

			string newdirname = dirp->d_name;
			if (newdirname == "." || newdirname == "..") {
				continue;
			}

			directories.push_back(new GenericFileSystemDirectory((GenericFileSystem*)(getFileSystem()), this, newdirname));
		}

		closedir(dp);
	}

	return directories;
}


FileList GenericFileSystemDirectory::getFiles() {
	FileList files;
	struct dirent *dirp;
	DIR *dp;

	if ((dp = opendir((getFullPath() + "/").c_str())) != NULL) {
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_type == DT_REG) {
				files.push_back(new GenericFileSystemFile(this, dirp->d_name));
			}
		}

		closedir(dp);
	}

	return files;
}







GenericFileSystemFile::GenericFileSystemFile(GenericFileSystemDirectory *parent, const string &name)
: File(parent), name(name)
{
	return;
}

GenericFileSystemFile::~GenericFileSystemFile() {
	return;
}


string GenericFileSystemFile::getName() const {
	return name;
}


DataBuffer *GenericFileSystemFile::getContent() {
	// TODO: implement me
	return NULL;
}

