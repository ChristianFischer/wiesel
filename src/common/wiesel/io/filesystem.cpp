/*
 * filesystem.cpp
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
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
