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
#include <string.h>

using namespace wiesel;



DataBuffer::DataBuffer() {
	return;
}

DataBuffer::~DataBuffer() {
	return;
}





ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(data_t data, size_t size) {
	unsigned char *new_buffer = new unsigned char[size];
	memcpy(new_buffer, data, size);
	return new ExclusiveDataBuffer(new_buffer, size);
}


ExclusiveDataBuffer::ExclusiveDataBuffer(data_t data, size_t size)
: data(data), size(size)
{
	return;
}

ExclusiveDataBuffer::~ExclusiveDataBuffer() {
	if (data) {
		delete data;
	}
}

ExclusiveDataBuffer::data_t ExclusiveDataBuffer::getData() const {
	return data;
}

size_t ExclusiveDataBuffer::getSize() const {
	return size;
}





SharedDataBuffer::SharedDataBuffer(data_t data, size_t size)
: data(data), size(size)
{
	return;
}

SharedDataBuffer::~SharedDataBuffer() {
	// data is an external reference, so it will not be destroyed!
}

SharedDataBuffer::data_t SharedDataBuffer::getData() const {
	return data;
}

size_t SharedDataBuffer::getSize() const {
	return size;
}

