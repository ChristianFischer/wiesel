/*
 * directory_filesystem.cpp
 *
 *  Created on: 16.04.2012
 *      Author: Baldur
 */

#include "directory_filesystem.h"

using namespace wiesel;
using namespace std;


DirectoryFileSystem::DirectoryFileSystem(Directory *enclosed_root) {
	this->enclosed_root = enclosed_root;
	this->enclosed_root->retain();

	this->root = new DirectoryFileSystemRootDirectory(this, enclosed_root);
	this->root->retain();

	return;
}

DirectoryFileSystem::~DirectoryFileSystem() {
	root->release();
	enclosed_root->release();
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
	enclosed_directory->retain();
	return;
}

DirectoryFileSystemDirectory::~DirectoryFileSystemDirectory() {
	enclosed_directory->release();
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
	FileList::size_type n = files.size();

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
	enclosed_file->retain();
	return;
}

DirectoryFileSystemFile::~DirectoryFileSystemFile() {
	enclosed_file->release();
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


DataBuffer *DirectoryFileSystemFile::getContent() {
	return getEnclosedFile()->getContent();
}

