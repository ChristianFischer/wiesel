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

#include "wiesel/io/datasource.h"
#include "wiesel/io/directory_filesystem.h"
#include "wiesel/util/log.h"

#include <direct.h>
#include <string.h>
#include <sstream>


using namespace wiesel;
using namespace std;


GenericPlatform::GenericPlatform() {
	// create file systems
	root_fs = new GenericFileSystem();
	asset_fs = NULL;

	// get the current working directory
	const int max_path_len = 4096;
	char working_dir_name[max_path_len];
	_getcwd(working_dir_name, max_path_len);

	// replace backslashes with normal slashes, as the file system API
	// cannot handle backslashes as separators yet.
	for(int i=0; i<max_path_len; i++) {
		if (working_dir_name[i] == '\0') {
			break;
		}

		if (working_dir_name[i] == '\\') {
			working_dir_name[i] =  '/';
		}
	}

	if (strlen(working_dir_name)) {
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
	delete asset_fs;
	delete root_fs;

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


FileSystem *GenericPlatform::getRootFileSystem() {
	return root_fs;
}


FileSystem *GenericPlatform::getAssetFileSystem() {
	return asset_fs;
}
