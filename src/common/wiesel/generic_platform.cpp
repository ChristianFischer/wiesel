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
#include "generic_platform.h"
#include "wiesel/io/generic_root_fs.h"
#include "wiesel/platform-fileutils.h"

#include "wiesel/io/datasource.h"
#include "wiesel/io/directory_filesystem.h"
#include "wiesel/util/log.h"

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <map>


using namespace wiesel;
using namespace std;


GenericPlatform::GenericPlatform() {
	// create file systems
	root_fs = new GenericFileSystem();
	asset_fs = NULL;

	// get the current working directory
	std::string working_dir_name = getWorkingDirName();
	if (working_dir_name.empty() == false) {
		stringstream ss;
		ss << working_dir_name;
		ss << "/resources/common";

		Directory *asset_root = root_fs->findDirectory(ss.str());
		if (asset_root) {
			asset_fs = new DirectoryFileSystem(asset_root);
		}
	}

	return;
}

GenericPlatform::~GenericPlatform() {
	for(std::map<std::string,FileSystem*>::iterator it=data_fs.begin(); it!=data_fs.end(); it++) {
		delete it->second;
	}
	data_fs.clear();

	if (asset_fs) {
		delete asset_fs;
	}

	if (root_fs) {
		delete root_fs;
	}

	return;
}


bool GenericPlatform::onInit() {
	return true;
}


void GenericPlatform::onShutdown() {
	return;
}


void GenericPlatform::onRunFirst() {
	return;
}


bool GenericPlatform::onRun() {
	return false;
}


std::string GenericPlatform::getEnv(const std::string &name) const {
	#if defined(_MSC_VER)
		size_t ret;
		const size_t size = 1024;
		char buffer[size];

		getenv_s(&ret, buffer, size, name.c_str());
		if (ret) {
			return buffer;
		}
	#else
		const char *result = getenv(name.c_str());
		if (result) {
			return std::string(result);
		}
	#endif

	return std::string();
}


std::string GenericPlatform::getWorkingDirName() const {
	const int max_path_len = 4096;
	char working_dir_name[max_path_len];

	#if WIESEL_PLATFORM_WINDOWS
		_getcwd(working_dir_name, max_path_len);
	#else
		getcwd(working_dir_name, max_path_len);
	#endif

	if (strlen(working_dir_name)) {
		std::string str = working_dir_name;
		convertPathToEngineStyle(&str);
		return str;
	}

	return std::string();
}


std::string GenericPlatform::getHomeDirName() const {
	std::string homedir;

	#if WIESEL_PLATFORM_WINDOWS
		homedir = getEnv("USERPROFILE");
		if (homedir.empty() == false) {
			convertPathToEngineStyle(&homedir);
			return homedir;
		}
	#endif

	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		struct passwd *pw = getpwuid(getuid());
		if (pw->pw_dir) {
			std::string home_dir = pw->pw_dir;
			convertPathToEngineStyle(&home_dir);
			return home_dir;
		}
	#endif

	return std::string();
}


std::string GenericPlatform::getAppDataDirName() const {
	#if WIESEL_PLATFORM_WINDOWS
		std::string appdata = getEnv("APPDATA");
		if (appdata.empty() == false) {
			convertPathToEngineStyle(&appdata);
			return appdata;
		}
	#endif // WIESEL_PLATFORM_WINDOWS

	#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
		std::string homedir = getHomeDirName();
		if (homedir.empty() == false) {
			return homedir;
		}
	#endif // WIESEL_PLATFORM_UNIX

	return std::string();
}


void GenericPlatform::convertPathToEngineStyle(std::string* path) {
	// replace backslashes with normal slashes, as the file system API
	// cannot handle backslashes as separators yet.
	for(std::string::iterator it=path->begin(); it!=path->end(); it++) {
		if (*it == '\\') {
			*it =  '/';
		}
	}

	return;
}


FileSystem *GenericPlatform::getRootFileSystem() {
	return root_fs;
}


FileSystem *GenericPlatform::getAssetFileSystem() {
	return asset_fs;
}


FileSystem *GenericPlatform::getDataFileSystem(const std::string &subdir) {
	// no nested subdirs allowed
	assert(subdir.find('\\') == string::npos);
	assert(subdir.find('/') == string::npos);

	std::map<std::string,FileSystem*>::iterator it=data_fs.find(subdir);
	if (it != data_fs.end()) {
		return it->second;
	}
	else {
		FileSystem* fs = NULL;

		// get the user's application data directory
		std::string appdata_dir_name = getAppDataDirName();
		if (appdata_dir_name.empty() == false) {
			Directory *appdata_root = root_fs->findDirectory(appdata_dir_name);
			if (appdata_root) {
				Directory *appdata_subdir = NULL;

				#if WIESEL_PLATFORM_WINDOWS
					if (appdata_subdir == NULL) {
						appdata_subdir = appdata_root->createDirectory(subdir);
					}
				#endif

				#if WIESEL_PLATFORM_UNIX || WIESEL_PLATFORM_CYGWIN
					if (appdata_subdir == NULL) {
						// on unix, we're adding a leading dot before the directory name
						// to hide the application data in the user's home
						std::string unix_subdir = std::string(".") + subdir;
						appdata_subdir = appdata_root->createDirectory(unix_subdir);
					}
				#endif

				if (appdata_subdir) {
					fs = new DirectoryFileSystem(appdata_subdir);
				}
			}
		}

		data_fs[subdir] = fs;
		return fs;
	}

	return NULL;
}


FileSystem *GenericPlatform::getExternalDataFileSystem(const std::string &subdir) {
	return getDataFileSystem(subdir);
}
