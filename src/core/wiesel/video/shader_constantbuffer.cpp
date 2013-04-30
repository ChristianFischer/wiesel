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
#include "shader_constantbuffer.h"
#include "video_driver.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <limits>


using namespace wiesel;
using namespace wiesel::video;
using namespace std;



ShaderConstantBufferTemplate::ShaderConstantBufferTemplate() {
	this->size	= 0;
	return;
}


ShaderConstantBufferTemplate::~ShaderConstantBufferTemplate() {
	return;
}


bool ShaderConstantBufferTemplate::addEntry(ValueType type, size_t elements, const std::string& name) {
	size_t offset = 0;

	// check, if there's already an entry with the same name
	for(EntryList::iterator it=entries.begin(); it!=entries.end(); it++) {
		if (it->name == name) {
			return false;
		}

		offset += it->elements * getTypeSize(it->type);
	}

	Entry entry;
	entry.type		= type;
	entry.elements	= elements;
	entry.offset	= offset;
	entry.name		= name;

	entries.push_back(entry);

	// update the buffer's size
	this->size = offset + (entry.elements * getTypeSize(entry.type));

	return true;
}


const ShaderConstantBufferTemplate::Entry *ShaderConstantBufferTemplate::findEntry(const std::string& name) const {
	// check, if there's already an entry with the same name
	for(EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
		if (it->name == name) {
			return &(*it);
		}
	}

	return NULL;
}





ShaderConstantBufferWriter::ShaderConstantBufferWriter() {
	return;
}

ShaderConstantBufferWriter::~ShaderConstantBufferWriter() {
	return;
}


bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, int32_t i) {
	return setShaderValue(name, TypeInt32, 1, &i);
}

bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, float f) {
	return setShaderValue(name, TypeFloat, 1, &f);
}

bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, const vector2d &v) {
	return setShaderValue(name, TypeVector2f, 1, (const float*)v);
}

bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, const vector3d &v) {
	return setShaderValue(name, TypeVector3f, 1, (const float*)v);
}

bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, const matrix4x4 &m) {
	return setShaderValue(name, TypeMatrix4x4f, 1, (const float*)m);
}


bool ShaderConstantBufferWriter::setShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue) {
	return doSetShaderValue(name, type, elements, pValue);
}




ShaderConstantBuffer::ShaderConstantBuffer() {
	this->buffer_template	= NULL;
	this->data				= NULL;
	this->change_ver		= 1;
	return;
}


ShaderConstantBuffer::ShaderConstantBuffer(ShaderConstantBufferTemplate *buffer_template) {
	this->buffer_template	= buffer_template;
	this->data				= NULL;
	this->change_ver		= 1;

	if (this->buffer_template) {
		keep(this->buffer_template);

		size_t buffer_size = this->buffer_template->getSize();
		this->data = reinterpret_cast<data_t>(malloc(buffer_size));
		assert(this->data);

		// zero memory
		memset(this->data, '\0', buffer_size);
	}

	return;
}


ShaderConstantBuffer::~ShaderConstantBuffer() {
	if (data) {
		free(data);
	}

	return;
}



bool ShaderConstantBuffer::writeDataAtOffset(size_t offset, const void *pValue, size_t size) {
	if (
			getTemplate()
		&&	((offset + size) <= getTemplate()->getSize())
	) {
		memcpy(data + offset, pValue, size);
		incrementChangeVersion();
		return true;
	}

	return false;
}


bool ShaderConstantBuffer::doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue) {
	if (getTemplate()) {
		for(
				ShaderConstantBufferTemplate::EntryList::const_iterator
				it  = getTemplate()->getEntries()->begin();
				it != getTemplate()->getEntries()->end();
				it++
		) {
			if (it->name == name) {
				size_t size = elements * getTypeSize(type);
				return writeDataAtOffset(it->offset, pValue, size);
			}
		}
	}

	return false;
}


void ShaderConstantBuffer::incrementChangeVersion() {
	if (change_ver == std::numeric_limits<version_t>::max()) {
		change_ver = 1;
	}
	else {
		++change_ver;
	}
}



bool ShaderConstantBuffer::doLoadContent() {
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

	ShaderConstantBufferContent *rc = driver->createConstantBufferContent(this);
	if (rc == NULL) {
		return false;
	}

	setContent(rc);

	return true;
}


bool ShaderConstantBuffer::doUnloadContent() {
	setContent(NULL);
	return true;
}





ShaderConstantBufferContent::ShaderConstantBufferContent() {
	return;
}

ShaderConstantBufferContent::ShaderConstantBufferContent(ShaderConstantBuffer *shader_constant_buffer) {
	this->shader_constant_buffer = shader_constant_buffer;
	return;
}

ShaderConstantBufferContent::~ShaderConstantBufferContent() {
	return;
}

