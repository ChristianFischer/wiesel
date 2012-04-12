/*
 * directory.cpp
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
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
	safe_retain(this->parent);
	return;
}

Directory::~Directory() {
	// release parent directory
	safe_release(parent);
	return;
}


string Directory::getFullPath() {
	string path;

	if (getParent()) {
		return getParent()->getFullPath() + "/" + getName();
	}

	return getName();
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

	DirectoryList directories = getSubDirectories();
	for(DirectoryList::iterator it=directories.begin(); it!=directories.end(); it++) {
		Directory *dir = *it;

		if (dir->getName() == current_entry) {
			return dir->findDirectory(next_entries);
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


void Directory::sortByName(DirectoryList &list, bool asc) {
	list.sort(asc ? DirectorySortByNameAscPredicate : DirectorySortByNameDescPredicate);
}
