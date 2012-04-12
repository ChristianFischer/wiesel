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
	DataBuffer *buffer = getContent();
	const char *data = reinterpret_cast<const char*>(buffer->getData());
	size_t size = buffer->getSize();
	return string(data, size);
}


vector<string> File::getLines() {
	DataBuffer *buffer = getContent();
	const char *data = reinterpret_cast<const char*>(buffer->getData());
	size_t size = buffer->getSize();
	vector<string> lines;

	const char *file_end   = data + size;
	const char *line_begin = data;
	for(; data<=file_end; data++) {
		if (data >= file_end || *data == '\r' || *data == '\n') {
			size_t line_length = data - line_begin;
			lines.push_back(string(line_begin, line_length));

			// next line start
			line_begin = data + 1;

			// handle windows style linebreaks
			if (line_begin < file_end && *data == '\r' && *line_begin == '\n') {
				line_begin++;
				data++;
			}
		}
	}

	return lines;
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
