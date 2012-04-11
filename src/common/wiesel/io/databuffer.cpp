/*
 * databuffer.cpp
 *
 *  Created on: 11.04.2012
 *      Author: Baldur
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





ExclusiveDataBuffer *ExclusiveDataBuffer::createCopyOf(const data_t data, size_t size) {
	data_t new_buffer = new unsigned char[size];
	memcpy(new_buffer, data, size);
	return new ExclusiveDataBuffer(new_buffer, size);
}


ExclusiveDataBuffer::ExclusiveDataBuffer(const data_t data, size_t size)
: data(data), size(size)
{
	return;
}

ExclusiveDataBuffer::~ExclusiveDataBuffer() {
	if (data) {
		delete data;
	}
}

const ExclusiveDataBuffer::data_t ExclusiveDataBuffer::getData() const {
	return data;
}

size_t ExclusiveDataBuffer::getSize() const {
	return size;
}





SharedDataBuffer::SharedDataBuffer(const data_t data, size_t size)
: data(data), size(size)
{
	return;
}

SharedDataBuffer::~SharedDataBuffer() {
	// data is an external reference, so it will not be destroyed!
}

const SharedDataBuffer::data_t SharedDataBuffer::getData() const {
	return data;
}

size_t SharedDataBuffer::getSize() const {
	return size;
}

