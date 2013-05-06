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
#include "wiesel/platform_config.h"
#include "generic_root_fs.h"
#include "wiesel/util/log.h"
#include "wiesel/platform-fileutils.h"

#include <stdio.h>
#include <sstream>
#include <fstream>


using namespace wiesel;
using namespace std;



#if defined(_MSC_VER)
	static std::wstring str2wstr(const std::string& str) {
		wchar_t *buffer = new wchar_t[str.length() + 1];
		buffer[str.size()] = '\0';
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, (int)str.length());

		std::wstring result = buffer;
		delete[] buffer;

		return result;
	}


	static std::string wstr2str(const std::wstring& wstr) {
		char *buffer = new char[wstr.length() + 1];
		buffer[wstr.size()] = '\0';
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buffer, (int)wstr.length(), NULL, NULL);

		std::string result = buffer;
		delete[] buffer;

		return result;
	}
#endif


GenericFileSystem::GenericFileSystem() {
	root = new GenericFileSystemDirectory(this, NULL, "");
	keep(root);
	return;
}

GenericFileSystem::GenericFileSystem(const std::string &root_path) {
	root = new GenericFileSystemDirectory(this, NULL, root_path);
	keep(root);
	return;
}

GenericFileSystem::~GenericFileSystem() {
	safe_release(root);
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


string GenericFileSystemDirectory::getFullPath() const {
	#if WIESEL_PLATFORM_WINDOWS && !WIESEL_PLATFORM_CYGWIN
	if (getParent() && getParent()->getName().empty()) {
		// this is a 2nd level directory, on a windows file system this
		// would be a drive-letter, so we need to skip the leading slash
		return getName();
	}
	#endif // windows

	return Directory::getFullPath();
}


string GenericFileSystemDirectory::getNativePath() const {
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
	DirectoryList directories;

	#if defined(_MSC_VER)
		string fullpath = (getFullPath() + "/*");
		HANDLE hFind;
		WIN32_FIND_DATA ffd;

		if ((hFind = FindFirstFile(str2wstr(fullpath).c_str(), &ffd)) != INVALID_HANDLE_VALUE){
			do {
				if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					continue;
				}

				directories.push_back(new GenericFileSystemDirectory(fs, this, wstr2str(ffd.cFileName)));
			}
			while(FindNextFile(hFind, &ffd));

			FindClose(hFind);
		}
	#else
		string fullpath = (getFullPath() + "/");
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

		if ((hFind = FindFirstFile(str2wstr(getFullPath() + "/*").c_str(), &ffd)) != INVALID_HANDLE_VALUE){
			do {
				if((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
					continue;
				}

				files.push_back(new GenericFileSystemFile(this, wstr2str(ffd.cFileName)));
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


bool GenericFileSystemDirectory::canRead() const {
	struct stat fileinfo;

	if (stat(getFullPath().c_str(), &fileinfo) < 0) {
		return false;
	}

	#if WIESEL_PLATFORM_UNIX | WIESEL_PLATFORM_CYGWIN
		if (fileinfo.st_mode & S_IRUSR) {
			return true;
		}
	#endif

	#if WIESEL_PLATFORM_WINDOWS
		if (fileinfo.st_mode & S_IREAD) {
			return true;
		}
	#endif

	return false;
}


bool GenericFileSystemDirectory::canWrite() const {
	struct stat fileinfo;

	if (stat(getFullPath().c_str(), &fileinfo) < 0) {
		return false;
	}

	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		if (fileinfo.st_mode & S_IWUSR) {
			return true;
		}
	#endif

	#if WIESEL_PLATFORM_WINDOWS
		if (fileinfo.st_mode & S_IWRITE) {
			return true;
		}
	#endif

	return false;
}


Directory *GenericFileSystemDirectory::doCreateDirectory(const string &name) {
	// no nested directories allowed
	assert(name.find('/') == string::npos);

	stringstream ss;
	ss << getFullPath();
	ss << '/';
	ss << name;
	string fullpath = ss.str();

	// now create the directory
	int result;
	
	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		result = mkdir(fullpath.c_str(), 0777);
	#else
		result = mkdir(fullpath.c_str());
	#endif

	if (result != 0) {
		return NULL;
	}

	// after the directory was created, try again to find it
	return findDirectory(name);
}


File *GenericFileSystemDirectory::doCreateFile(const string &name) {
	// no nested directories allowed
	assert(name.find('/') == string::npos);

	stringstream ss;
	ss << getFullPath();
	ss << '/';
	ss << name;
	string fullpath = ss.str();

	// open the file for writing to create it
	ofstream file_out;
	file_out.open(fullpath.c_str());
	file_out.flush();
	file_out.close();

	// after the file was created, try again to find it
	return findFile(name);
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


string GenericFileSystemFile::getNativePath() const {
	return getFullPath();
}


DataBuffer *GenericFileSystemFile::loadContent() {
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


bool GenericFileSystemFile::canRead() const {
	struct stat fileinfo;

	if (stat(getFullPath().c_str(), &fileinfo) < 0) {
		return false;
	}

	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		if (fileinfo.st_mode & S_IRUSR) {
			return true;
		}
	#endif

	#if WIESEL_PLATFORM_WINDOWS
		if (fileinfo.st_mode & S_IREAD) {
			return true;
		}
	#endif

	return false;
}


bool GenericFileSystemFile::canWrite() const {
	struct stat fileinfo;

	if (stat(getFullPath().c_str(), &fileinfo) < 0) {
		return false;
	}

	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		if (fileinfo.st_mode & S_IWUSR) {
			return true;
		}
	#endif

	#if WIESEL_PLATFORM_WINDOWS
		if (fileinfo.st_mode & S_IWRITE) {
			return true;
		}
	#endif

	return false;
}

