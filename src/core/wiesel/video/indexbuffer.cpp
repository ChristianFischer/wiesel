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
#include "indexbuffer.h"
#include "screen.h"
#include "video_driver.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace std;


IndexBuffer::IndexBuffer() {
	this->num_entries	= 0;
	this->capacity		= 0;
	this->data			= NULL;

	setBytesPerElement(2);

	return;
}


IndexBuffer::IndexBuffer(int bytes) {
	this->num_entries	= 0;
	this->capacity		= 0;
	this->data			= NULL;

	setBytesPerElement(bytes);

	return;
}


IndexBuffer::~IndexBuffer() {
	clear();

	if (data) {
		free(data);
	}

	return;
}



void IndexBuffer::setBytesPerElement(int bytes) {
	assert(num_entries == 0);

	if (num_entries == 0) {
		bytes_per_entry = bytes;

		// clear the buffer, because the buffer needs to be resized.
		clear();
	}

	return;
}


int IndexBuffer::getBytesPerElement() const {
	return bytes_per_entry;
}


IndexBuffer::index_t IndexBuffer::setCapacity(index_t capacity) {
	if (this->data == NULL) {
		this->data = reinterpret_cast<data_t>(malloc(bytes_per_entry * capacity));
		assert(this->data);

		// clear the allocated memory
		memset(this->data, '\0', bytes_per_entry * capacity);

		if (this->data) {
			this->capacity = capacity;
		}
		else {
			this->capacity = 0;
		}
	}
	else {
		// do not change the capacity, if the new capacity is
		// smaller than the current number of vertices
		assert(capacity >= num_entries);
		if (capacity < num_entries) {
			return this->capacity;
		}

		data_t new_data = reinterpret_cast<data_t>(realloc(data, bytes_per_entry * capacity));
		assert(new_data);

		if (new_data) {
			if (data != new_data) {
				data  = new_data;
			}

			this->capacity = capacity;
		}
	}

	// hardware buffer needs to be re-created
	invalidateHardwareData();

	return this->capacity;
}


IndexBuffer::index_t IndexBuffer::ensureCapacity(index_t capacity) {
	if (this->capacity < capacity) {
		return setCapacity(capacity);
	}

	return this->capacity;
}


IndexBuffer::index_t IndexBuffer::getCapacity() const {
	return this->capacity;
}


IndexBuffer::index_t IndexBuffer::getSize() const {
	return this->num_entries;
}


void IndexBuffer::clear() {
	if (data) {
		free(data);
		data = NULL;
	}

	capacity     = 0;
	num_entries  = 0;

	// hardware data is now invalid
	invalidateHardwareData();

	return;
}





IndexBuffer::index_t IndexBuffer::addIndex(VertexBuffer::index_t entry) {
	index_t index = num_entries;
	index_t new_capacity = ensureCapacity(index + 1);

	if (new_capacity > index) {
		num_entries++;
		setIndex(index, entry);
	}

	return index;
}


void IndexBuffer::setIndex(index_t index, VertexBuffer::index_t value) {
	assert(index < num_entries);

	// check, if the value fits into the buffer
	assert(value <= (1 << (bytes_per_entry * 8)));

	if (index < num_entries) {
		switch(bytes_per_entry) {
			case 1: {
				(reinterpret_cast<uint8_t*>(data))[index] = static_cast<uint8_t>(value);
				break;
			}

			case 2: {
				(reinterpret_cast<uint16_t*>(data))[index] = static_cast<uint16_t>(value);
				break;
			}

			case 4: {
				(reinterpret_cast<uint32_t*>(data))[index] = static_cast<uint32_t>(value);
				break;
			}
		}
	}

	return;
}


void IndexBuffer::invalidateHardwareData() const {
	// TODO: invalidate hardware data
	return;
}



bool IndexBuffer::doLoadContent() {
	assert(getContent() == NULL);

	// get the video device
	Screen *screen = dynamic_cast<Screen*>(getDevice());
	if (screen == NULL) {
		return false;
	}

	VideoDeviceDriver *driver = screen->getVideoDeviceDriver();
	if (driver == NULL) {
		return false;
	}

	IndexBufferContent *rc = driver->createIndexBufferContent(this);
	if (rc == NULL) {
		return false;
	}

	setContent(rc);

	return true;
}


bool IndexBuffer::doUnloadContent() {
	setContent(NULL);
	return true;
}




IndexBufferContent::IndexBufferContent() {
	return;
}

IndexBufferContent::IndexBufferContent(IndexBuffer *index_buffer) {
	this->index_buffer = index_buffer;
	return;
}

IndexBufferContent::~IndexBufferContent() {
	return;
}
