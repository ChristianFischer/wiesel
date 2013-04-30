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
	keep(this->parent);
	return;
}

File::~File() {
	safe_release(parent);
	return;
}


string File::getFullPath() const {
	return getParent()->getFullPath() + "/" + getName();
}


string File::getNativePath() const {
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


std::string File::loadContentAsString() {
	ref<DataBuffer> buffer = loadContent();
	if (buffer) {
		const char *data = reinterpret_cast<const char*>(buffer->getData());
		size_t size = buffer->getSize();
		return string(data, size);
	}

	return "";
}


vector<string> File::loadLines() {
	ref<DataBuffer> buffer = loadContent();
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
	keep(file);
	this->content = NULL;
	return;
}

FileDataSource::~FileDataSource() {
	safe_release(file);
	safe_release(content);
}

DataBuffer *FileDataSource::getDataBuffer() {
	if (content == NULL) {
		content = keep(file->loadContent());
	}
		
	return content;
}

void FileDataSource::releaseDataBuffer() {
	safe_release(content);
}

File *FileDataSource::getFile() {
	return file;
}
