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



ShaderConstantBufferTemplateBase::ShaderConstantBufferTemplateBase() {
	this->size	= 0;
	return;
}


ShaderConstantBufferTemplateBase::~ShaderConstantBufferTemplateBase() {
	return;
}


const ShaderConstantBufferTemplateBase::Entry *ShaderConstantBufferTemplateBase::findEntry(const std::string& name) const {
	// check, if there's already an entry with the same name
	for(EntryList::const_iterator it=entries.begin(); it!=entries.end(); it++) {
		if (it->name == name) {
			return &(*it);
		}
	}

	return NULL;
}


ShaderConstantBufferTemplateBase::index_t ShaderConstantBufferTemplateBase::findShaderValueIndex(const std::string& name) const {
	for(index_t i=getEntries()->size(); --i>=0;) {
		if ((*(getEntries()))[i].name == name) {
			return i;
		}
	}

	return -1;
}





ShaderConstantBufferTemplate::ShaderConstantBufferTemplate() {
	return;
}


ShaderConstantBufferTemplate::~ShaderConstantBufferTemplate() {
	return;
}


ShaderConstantBuffer *ShaderConstantBufferTemplate::getSharedBuffer() {
	if (shared_buffer == NULL) {
		shared_buffer = new ShaderConstantBuffer(this);
	}

	return shared_buffer;
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


ShaderConstantBuffer::ShaderConstantBuffer(const ShaderConstantBufferTemplate *buffer_template) {
	assert(buffer_template);

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



bool ShaderConstantBuffer::setShaderValueAt(index_t index, int32_t i) {
	return setShaderValueAt(index, TypeInt32, 1, &i);
}

bool ShaderConstantBuffer::setShaderValueAt(index_t index, float f) {
	return setShaderValueAt(index, TypeFloat, 1, &f);
}

bool ShaderConstantBuffer::setShaderValueAt(index_t index, const vector2d &v) {
	return setShaderValueAt(index, TypeVector2f, 1, (const float*)v);
}

bool ShaderConstantBuffer::setShaderValueAt(index_t index, const vector3d &v) {
	return setShaderValueAt(index, TypeVector3f, 1, (const float*)v);
}

bool ShaderConstantBuffer::setShaderValueAt(index_t index, const matrix4x4 &m) {
	return setShaderValueAt(index, TypeMatrix4x4f, 1, (const float*)m);
}

bool ShaderConstantBuffer::setShaderValueAt(index_t index, ValueType type, size_t elements, const void *pValue) {
	assert(index >= 0);

	if (getTemplate()) {
		const ShaderConstantBufferTemplate::EntryList* entries = getTemplate()->getEntries();
		assert(index < static_cast<index_t>(entries->size()));

		return writeDataAtOffset(
					(*entries)[index].offset,
					pValue,
					elements * getTypeSize(type)
		);
	}

	return false;
}




const ShaderConstantBuffer::data_t ShaderConstantBuffer::getShaderDataPointer(const std::string &name, ValueType type, size_t elements) const {
	if (getTemplate()) {
		for(
				ShaderConstantBufferTemplate::EntryList::const_iterator
				it  = getTemplate()->getEntries()->begin();
				it != getTemplate()->getEntries()->end();
				it++
		) {
			if (it->name == name) {
				assert(it->type == type);
				assert(it->elements >= elements);

				return data + it->offset;
			}
		}
	}

	return NULL;
}


const ShaderConstantBuffer::data_t ShaderConstantBuffer::getShaderDataPointerAt(ShaderConstantBufferTemplate::index_t index) const {
	if (getTemplate()) {
		const ShaderConstantBufferTemplate::EntryList *entries = getTemplate()->getEntries();
		assert(index < static_cast<int>(entries->size()));

		if (index >= 0 && index < static_cast<int>(entries->size())) {
			return data + entries->at(index).offset;
		}
	}

	return NULL;
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

