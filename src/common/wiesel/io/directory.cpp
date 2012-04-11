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
	if (parent) {
		parent->retain();
	}

	return;
}

Directory::~Directory() {
	// release parent directory
	if (parent) {
		parent->release();
	}

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
	if (name.empty()) {
		// no file
		return NULL;
	}

	if (name.size() > 0 && name[0] == '/') {
		// ignore any leading slashes
		return findFile(name.substr(1));
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

	if (next_entries.empty()) {
		// last entry, now we search for files
		FileList files = getFiles();
		for(FileList::iterator it=files.begin(); it!=files.end(); it++) {
			File *file = *it;

			if (file->getName() == current_entry) {
				return file;
			}
		}
	}
	else {
		// search for subdirectories
		DirectoryList directories = getSubDirectories();
		for(DirectoryList::iterator it=directories.begin(); it!=directories.end(); it++) {
			Directory *dir = *it;

			if (dir->getName() == current_entry) {
				return dir->findFile(next_entries);
			}
		}
	}

	return NULL;
}


void Directory::sortByName(DirectoryList &list, bool asc) {
	list.sort(asc ? DirectorySortByNameAscPredicate : DirectorySortByNameDescPredicate);
}
