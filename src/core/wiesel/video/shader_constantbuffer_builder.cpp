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
#include "shader_constantbuffer_builder.h"


using namespace wiesel;
using namespace wiesel::video;



ShaderConstantBufferTemplateBuilder::ShaderConstantBufferTemplateBuilder() {
	return;
}


ShaderConstantBufferTemplateBuilder::~ShaderConstantBufferTemplateBuilder() {
	return;
}


bool ShaderConstantBufferTemplateBuilder::addEntry(ValueType type, size_t elements, const std::string& name) {
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


ShaderConstantBufferTemplate *ShaderConstantBufferTemplateBuilder::create() const {
	ShaderConstantBufferTemplate *buffer_template = new ShaderConstantBufferTemplate();
	buffer_template->entries	= this->entries;
	buffer_template->size		= this->size;

	return buffer_template;
}

