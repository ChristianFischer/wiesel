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
#include "datasource.h"

using namespace wiesel;



DataSource::DataSource() {
	return;
}

DataSource::~DataSource() {
	return;
}



BufferDataSource::BufferDataSource(DataBuffer *buffer)
: buffer(buffer)
{
	if (buffer) {
		buffer->retain();
	}
}

BufferDataSource::BufferDataSource(DataBuffer::data_t data, size_t size)
: buffer(new ExclusiveDataBuffer(data, size))
{
	buffer->retain();
}

BufferDataSource::~BufferDataSource() {
	if (buffer) {
		buffer->release();
	}
}

DataBuffer *BufferDataSource::getDataBuffer() {
	return buffer;
}

