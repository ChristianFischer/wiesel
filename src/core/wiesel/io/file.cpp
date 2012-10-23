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


string File::getNativePath() {
	return "";
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
	if (buffer) {
		const char *data = reinterpret_cast<const char*>(buffer->getData());
		size_t size = buffer->getSize();
		return string(data, size);
	}

	return "";
}


vector<string> File::getLines() {
	DataBuffer *buffer = getContent();
	if (buffer == NULL) {
		return vector<string>();
	}

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
