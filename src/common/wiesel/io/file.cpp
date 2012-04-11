/*
 * file.cpp
 *
 *  Created on: 09.04.2012
 *      Author: Baldur
 */

#include "file.h"
#include "directory.h"

using namespace wiesel;
using namespace std;


File::File(Directory *parent)
: parent(parent)
{
	safe_retain(this->parent);
	return;
}

File::~File() {
	safe_release(parent);
	return;
}


string File::getFullPath() {
	return getParent()->getFullPath() + "/" + getName();
}


std::string File::getExtension() const {
	const string filename = getName();
	string::size_type dot_pos = filename.rfind('.');
	if (dot_pos == string::npos) {
		return "";
	}

	// get file extension
	string file_ext = filename.substr(dot_pos + 1);
	return file_ext;
}


DataSource *File::asDataSource() {
	return new FileDataSource(this);
}


std::string File::getContentAsString() {
	// TODO: implement me
	return "";
}


vector<string> File::getLines() {
	// TODO: implement me
	return vector<string>();
}


void File::sortByName(FileList &list, bool asc) {
	list.sort(asc ? FileSortByNameAscPredicate : FileSortByNameDescPredicate);
}






FileDataSource::FileDataSource(File *file)
: file(file)
{
	if (file) {
		file->retain();
	}
}

FileDataSource::~FileDataSource() {
	if (file) {
		file->release();
	}
}

DataBuffer *FileDataSource::getDataBuffer() {
	return file->getContent();
}

File *FileDataSource::getFile() {
	return file;
}
