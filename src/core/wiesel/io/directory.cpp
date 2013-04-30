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
#include "wiesel/util/log.h"

#include "directory.h"
#include <algorithm>
#include <vector>
#include <list>

using namespace wiesel;
using namespace std;


Directory::Directory(FileSystem *fs, Directory *parent)
: fs(fs), parent(parent)
{
	keep(this->parent);
	return;
}

Directory::~Directory() {
	// release parent directory
	safe_release(parent);
	return;
}


string Directory::getFullPath() const {
	if (getParent()) {
		return getParent()->getFullPath() + "/" + getName();
	}

	return getName();
}


string Directory::getNativePath() const {
	return "";
}


Directory *Directory::findDirectory(const std::string &name) {
	if (name.empty()) {
		// we have found our target!
		return this;
	}

	if (name.size() > 0 && name[0] == '/') {
		// ignore any leading slashes
		return findDirectory(name.substr(1));
	}

	string::size_type slash_pos = name.find('/');
	string current_entry;
	string next_entries;

	if (slash_pos == string::npos) {
		current_entry = name;
	}
	else {
		current_entry = name.substr(0, slash_pos);
		next_entries  = name.substr(slash_pos + 1);
	}

	// '.' is the current directory
	if (current_entry == ".") {
		return findDirectory(next_entries);
	}

	// '..' is the parent directory
	if (current_entry == "..") {
		if (getParent()) {
			return getParent()->findDirectory(next_entries);
		}

		// no parent? fail!
		return NULL;
	}

	Directory *subdir = getSubDirectory(current_entry);
	if (subdir) {
		return subdir->findDirectory(next_entries);
	}

	return NULL;
}


Directory *Directory::getSubDirectory(const string &name) {
	// no slashes allowed - to find relative paths, use findDirectory()
	assert(name.find('/') == string::npos);

	DirectoryList directories = getSubDirectories();
	for(DirectoryList::iterator it=directories.begin(); it!=directories.end(); it++) {
		Directory *dir = *it;

		if (dir->getName() == name) {
			autorelease(dir);
			return dir;
		}
	}

	return NULL;
}


File *Directory::findFile(const std::string &name) {
	string::size_type slash_pos = name.rfind('/');
	string dirname;
	string filename;

	if (slash_pos == string::npos) {
		dirname  = "";
		filename = name;
	}
	else {
		dirname  = name.substr(0, slash_pos);
		filename = name.substr(slash_pos + 1);
	}

	if (dirname.empty()) {
		// last entry, now we search for files
		FileList files = getFiles();
		for(FileList::iterator it=files.begin(); it!=files.end(); it++) {
			File *file = *it;

			if (file->getName() == filename) {
				autorelease(file);
				return file;
			}
		}
	}
	else {
		Directory *dir = findDirectory(dirname);
		if (dir) {
			return dir->findFile(filename);
		}
	}

	return NULL;
}


Directory *Directory::createDirectory(const std::string &name) {
	if (name.empty()) {
		// we have found our target!
		return this;
	}

	if (name.size() > 0 && name[0] == '/') {
		// ignore any leading slashes
		return findDirectory(name.substr(1));
	}

	string::size_type slash_pos = name.find('/');
	string current_entry;
	string next_entries;

	if (slash_pos == string::npos) {
		current_entry = name;
	}
	else {
		current_entry = name.substr(0, slash_pos);
		next_entries  = name.substr(slash_pos + 1);
	}

	// does the current subdirectory exist?
	Directory *subdir = findDirectory(current_entry);

	// not found? Try to create
	if (subdir == NULL) {
		subdir = doCreateDirectory(current_entry);
	}

	if (subdir) {
		return subdir->createDirectory(next_entries);
	}

	return NULL;
}


File *Directory::createFile(const std::string &name) {
	string::size_type slash_pos = name.rfind('/');

	if (slash_pos == string::npos) {
		// we're in the right directory
		File *file = findFile(name);

		// create the file, if it doesn't exist
		if (file == NULL) {
			file = doCreateFile(name);
		}

		return file;
	}
	else {
		string dirname  = name.substr(0, slash_pos);
		string filename = name.substr(slash_pos + 1);

		// find the file's directory
		Directory *subdir = findDirectory(dirname);
		if (subdir) {
			return subdir->createFile(filename);
		}
	}

	return NULL;
}


void Directory::sortByName(DirectoryList &list, bool asc) {
	list.sort(asc ? DirectorySortByNameAscPredicate : DirectorySortByNameDescPredicate);
}
