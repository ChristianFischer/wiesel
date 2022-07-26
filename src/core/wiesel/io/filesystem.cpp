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
#include "filesystem.h"

using namespace wiesel;
using namespace std;


FileSystem::FileSystem() {
	return;
}

FileSystem::~FileSystem() {
	return;
}


Directory *FileSystem::findDirectory(const std::string &name) {
	Directory *root = getRootDirectory();
	if (root) {
		return root->findDirectory(name);
	}

	return NULL;
}


File *FileSystem::findFile(const std::string &name) {
	Directory *root = getRootDirectory();
	if (root) {
		return root->findFile(name);
	}

	return NULL;
}


Directory *FileSystem::createDirectory(const std::string &name) {
	Directory *root = getRootDirectory();
	if (root) {
		return root->createDirectory(name);
	}

	return NULL;
}


File *FileSystem::createFile(const std::string &name) {
	Directory *root = getRootDirectory();
	if (root) {
		return root->createFile(name);
	}

	return NULL;
}
