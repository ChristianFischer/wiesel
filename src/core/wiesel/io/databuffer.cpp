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
#include "databuffer.h"
#include <malloc.h>
#include <string.h>

using namespace wiesel;
using namespace std;



DataBuffer::DataBuffer() {
	return;
}

DataBuffer::~DataBuffer() {
	return;
}


const char *DataBuffer::getDataAsCharPtr() const {
	return reinterpret_cast<const char*>(getData());
}





ExclusiveDataBuffer *ExclusiveDataBuffer::create(size_t size) {
	mutable_data_t data = new unsigned char[size];
	if (data) {
		return new ExclusiveDataBuffer(data, size);
	}

	return NULL;
}


ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(data_t data, size_t size) {
	unsigned char *new_buffer = new unsigned char[size];
	memcpy(new_buffer, data, size);
	return new ExclusiveDataBuffer(new_buffer, size);
}


ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(const void* data, size_t size) {
	return createCopyOf(reinterpret_cast<data_t>(data), size);
}


ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(const char* data, size_t size) {
	return createCopyOf(reinterpret_cast<data_t>(data), size);
}


ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(const string& str) {
	return createCopyOf(str.c_str(), str.size() + 1);
}


ExclusiveDataBuffer::ExclusiveDataBuffer(mutable_data_t data, size_t size)
: data(data), size(size)
{
	return;
}

ExclusiveDataBuffer::~ExclusiveDataBuffer() {
	if (data) {
		delete data;
	}
}

ExclusiveDataBuffer::mutable_data_t ExclusiveDataBuffer::getMutableData() {
	return data;
}

ExclusiveDataBuffer::data_t ExclusiveDataBuffer::getData() const {
	return data;
}

size_t ExclusiveDataBuffer::getSize() const {
	return size;
}


bool ExclusiveDataBuffer::resize(size_t new_size) {
	mutable_data_t new_data = reinterpret_cast<mutable_data_t>(realloc(data, new_size));

	if (new_data) {
		data = new_data;
		return true;
	}

	return false;
}





SharedDataBuffer::SharedDataBuffer(data_t data, size_t size)
: data(data), size(size)
{
	return;
}

SharedDataBuffer::~SharedDataBuffer() {
	// data is an external reference, so it will not be destroyed!
}

SharedDataBuffer::mutable_data_t SharedDataBuffer::getMutableData() {
	// changing a shared buffer's content is forbidden, so we return a \c NULL pointer.
	return NULL;
}

SharedDataBuffer::data_t SharedDataBuffer::getData() const {
	return data;
}

size_t SharedDataBuffer::getSize() const {
	return size;
}

bool SharedDataBuffer::resize(size_t) {
	// resizing is not supported, because the data is shared with other code
	return false;
}

