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
#include "wiesel.h"
#include "generic_root_fs.h"
#include "wiesel/util/log.h"
#include <direct.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sstream>

#if WIESEL_PLATFORM_WINDOWS && !WIESEL_PLATFORM_CYGWIN
#include "windows.h"
#endif

#if defined(_MSC_VER)
#else
#include <dirent.h>
#endif



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


string GenericFileSystemDirectory::getFullPath() {
	#if WIESEL_PLATFORM_WINDOWS && !WIESEL_PLATFORM_CYGWIN
	if (getParent() && getParent()->getName().empty()) {
		// this is a 2nd level directory, on a windows file system this
		// would be a drive-letter, so we need to skip the leading slash
		return getName();
	}
	#endif // windows

	return Directory::getFullPath();
}


string GenericFileSystemDirectory::getNativePath() {
	return getFullPath();
}


DirectoryList GenericFileSystemDirectory::getSubDirectories() {
	#if WIESEL_PLATFORM_WINDOWS && !WIESEL_PLATFORM_CYGWIN
	// on windows, the root folder should contain a list of windows drive letters
	if (this->name.empty()) {
		GenericFileSystem *fs = (GenericFileSystem*)(getFileSystem());
		DirectoryList directories;
		DWORD all_drives = GetLogicalDrives();

		for(char drive=0; drive<=26; drive++) {
			if ((all_drives & (1 << drive)) != 0) {
				stringstream drive_name;
				drive_name << (char)('A' + drive);
				drive_name << ':';
				directories.push_back(new GenericFileSystemDirectory(fs, this, drive_name.str()));
			}
		}

		return directories;
	}
	#endif // windows

	GenericFileSystem *fs = dynamic_cast<GenericFileSystem*>(getFileSystem());
	string fullpath = (getFullPath() + "/");
	DirectoryList directories;

	#if defined(_MSC_VER)
		HANDLE hFind;
		WIN32_FIND_DATA ffd;

		if ((hFind = FindFirstFile(fullpath.c_str(), &ffd)) != INVALID_HANDLE_VALUE){
			do {
				if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					continue;
				}

				directories.push_back(new GenericFileSystemDirectory(fs, this, ffd.cFileName));
			}
			while(FindNextFile(hFind, &ffd));

			FindClose(hFind);
		}
	#else
		struct dirent *dirp;
		struct stat fileinfo;
		DIR *dp;

		if ((dp = opendir(fullpath.c_str())) != NULL) {
			while ((dirp = readdir(dp)) != NULL) {
				string newdirname = dirp->d_name;
				string fullpath = getFullPath() + "/" + newdirname;

				// get entry fileinfo
				if (stat(fullpath.c_str(), &fileinfo) < 0) {
					continue;
				}

				// skip, if is no directory
				if (S_ISDIR(fileinfo.st_mode) == false) {
					continue;
				}

				// skip "current" and "parent" directory entries
				if (newdirname == "." || newdirname == "..") {
					continue;
				}

				directories.push_back(new GenericFileSystemDirectory(fs, this, newdirname));
			}

			closedir(dp);
		}
	#endif

	return directories;
}


FileList GenericFileSystemDirectory::getFiles() {
	FileList files;

	#if defined(_MSC_VER)
		HANDLE hFind;
		WIN32_FIND_DATA ffd;

		if ((hFind = FindFirstFile((getFullPath() + "/").c_str(), &ffd)) != INVALID_HANDLE_VALUE){
			do {
				if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
					continue;
				}

				files.push_back(new GenericFileSystemFile(this, ffd.cFileName));
			}
			while(FindNextFile(hFind, &ffd));

			FindClose(hFind);
		}
	#else
		struct dirent *dirp;
		struct stat fileinfo;
		DIR *dp;

		if ((dp = opendir((getFullPath() + "/").c_str())) != NULL) {
			while ((dirp = readdir(dp)) != NULL) {
				string newfilename = dirp->d_name;
				string fullpath = getFullPath() + "/" + newfilename;

				// get entry fileinfo
				if (stat(fullpath.c_str(), &fileinfo) < 0) {
					continue;
				}

				// skip, if not a file
				if (S_ISREG(fileinfo.st_mode) == false) {
					continue;
				}

				files.push_back(new GenericFileSystemFile(this, newfilename));
			}

			closedir(dp);
		}
	#endif

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


string GenericFileSystemFile::getNativePath() {
	return getFullPath();
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

