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
#include "directory_filesystem.h"

using namespace wiesel;
using namespace std;


DirectoryFileSystem::DirectoryFileSystem(Directory *enclosed_root) {
	this->enclosed_root = enclosed_root;
	keep(enclosed_root);

	this->root = new DirectoryFileSystemRootDirectory(this, enclosed_root);
	keep(this->root);

	return;
}

DirectoryFileSystem::~DirectoryFileSystem() {
	clear_ref(root);
	clear_ref(enclosed_root);
	return;
}


Directory *DirectoryFileSystem::getRootDirectory() {
	return root;
}


Directory *DirectoryFileSystem::getEnclosedRootDirectory() {
	return enclosed_root;
}


const Directory *DirectoryFileSystem::getEnclosedRootDirectory() const {
	return enclosed_root;
}







DirectoryFileSystemDirectory::DirectoryFileSystemDirectory(DirectoryFileSystem *fs, DirectoryFileSystemDirectory *parent, Directory *enclosed_directory)
:	Directory(fs, parent), enclosed_directory(enclosed_directory)
{
	keep(enclosed_directory);
	return;
}

DirectoryFileSystemDirectory::~DirectoryFileSystemDirectory() {
	clear_ref(enclosed_directory);
	return;
}


Directory *DirectoryFileSystemDirectory::getEnclosedDirectory() {
	return enclosed_directory;
}


const Directory *DirectoryFileSystemDirectory::getEnclosedDirectory() const {
	return enclosed_directory;
}


string DirectoryFileSystemDirectory::getName() const {
	return getEnclosedDirectory()->getName();
}


string DirectoryFileSystemDirectory::getNativePath() const {
	return getEnclosedDirectory()->getNativePath();
}


DirectoryList DirectoryFileSystemDirectory::getSubDirectories() {
	DirectoryFileSystem *fs = dynamic_cast<DirectoryFileSystem*>(getFileSystem());
	DirectoryList directories = getEnclosedDirectory()->getSubDirectories();

	for(DirectoryList::iterator it=directories.begin(); it!=directories.end(); it++) {
		// create a new wrapper object
		Directory *directory = new DirectoryFileSystemDirectory(fs, this, *it);

		// erase the current directory from the list
		it = directories.erase(it);

		// and insert the directory wrapper at the current position
		it = directories.insert(it, directory);
	}

	return directories;
}


FileList DirectoryFileSystemDirectory::getFiles() {
	FileList files = getEnclosedDirectory()->getFiles();

	for(FileList::iterator it=files.begin(); it!=files.end(); it++) {
		// create a new wrapper object
		File *file = new DirectoryFileSystemFile(this, *it);

		// erase the current file from the list
		it = files.erase(it);

		// and insert the file wrapper at the current position
		it = files.insert(it, file);
	}

	return files;
}


bool DirectoryFileSystemDirectory::canRead() const {
	if (enclosed_directory) {
		return enclosed_directory->canRead();
	}

	return false;
}


bool DirectoryFileSystemDirectory::canWrite() const {
	if (enclosed_directory) {
		return enclosed_directory->canWrite();
	}

	return false;
}



Directory *DirectoryFileSystemDirectory::doCreateDirectory(const string &name) {
	// no nested directories allowed
	assert(name.find('/') == string::npos);

	if (enclosed_directory) {
		Directory *native_dir = enclosed_directory->createDirectory(name);
		if (native_dir) {
			DirectoryFileSystem *fs = dynamic_cast<DirectoryFileSystem*>(getFileSystem());
			return new DirectoryFileSystemDirectory(fs, this, native_dir);
		}
	}

	return NULL;
}


File *DirectoryFileSystemDirectory::doCreateFile(const string &name) {
	// no nested directories allowed
	assert(name.find('/') == string::npos);

	if (enclosed_directory) {
		File *native_file = enclosed_directory->createFile(name);
		if (native_file) {
			return new DirectoryFileSystemFile(this, native_file);
		}
	}

	return NULL;
}







DirectoryFileSystemRootDirectory::DirectoryFileSystemRootDirectory(DirectoryFileSystem *fs, Directory *enclosed_directory)
:	DirectoryFileSystemDirectory(fs, NULL, enclosed_directory)
{
	return;
}

DirectoryFileSystemRootDirectory::~DirectoryFileSystemRootDirectory() {
	return;
}

string DirectoryFileSystemRootDirectory::getName() const {
	return "";
}







DirectoryFileSystemFile::DirectoryFileSystemFile(DirectoryFileSystemDirectory *parent, File *enclosed_file)
: File(parent), enclosed_file(enclosed_file)
{
	keep(enclosed_file);
	return;
}

DirectoryFileSystemFile::~DirectoryFileSystemFile() {
	clear_ref(enclosed_file);
	return;
}


File *DirectoryFileSystemFile::getEnclosedFile() {
	return enclosed_file;
}


const File *DirectoryFileSystemFile::getEnclosedFile() const {
	return enclosed_file;
}


string DirectoryFileSystemFile::getName() const {
	return getEnclosedFile()->getName();
}


string DirectoryFileSystemFile::getNativePath() const {
	return getEnclosedFile()->getNativePath();
}


DataBuffer *DirectoryFileSystemFile::loadContent() {
	return getEnclosedFile()->loadContent();
}


bool DirectoryFileSystemFile::canRead() const {
	if (enclosed_file) {
		return enclosed_file->canRead();
	}

	return false;
}


bool DirectoryFileSystemFile::canWrite() const {
	if (enclosed_file) {
		return enclosed_file->canWrite();
	}

	return false;
}
