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
	FILE *fp = fopen(getFullPath().c_str(), "rb");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		unsigned char *data = new unsigned char[size];
		if (data) {
			fread(data, size, 1, fp);
		}

		fclose(fp);

		return new ExclusiveDataBuffer(data, size);
	}

	return NULL;
}

