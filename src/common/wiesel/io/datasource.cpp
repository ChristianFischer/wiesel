/*
 * datasource.cpp
 *
 *  Created on: 11.04.2012
 *      Author: Baldur
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

