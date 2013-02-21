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
#include "gl_indexbuffer_content.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;


GlIndexBufferContent::GlIndexBufferContent(IndexBuffer *ib) : IndexBufferContent(ib) {
	this->handle		= 0;
	return;
}

GlIndexBufferContent::~GlIndexBufferContent() {
	releaseIndexBuffer();
}



GlIndexBufferContent *GlIndexBufferContent::createContentFor(IndexBuffer* ib) {
	GlIndexBufferContent *gl_ib = new GlIndexBufferContent(ib);
	gl_ib->initializeIndexBuffer();
	return gl_ib;
}


void GlIndexBufferContent::initializeIndexBuffer() {
	if (handle == 0) {
		int							bytes_per_entry	= getIndexBuffer()->getBytesPerElement();
		IndexBuffer::index_t		num_entries		= getIndexBuffer()->getSize();
		const IndexBuffer::data_t	data			= getIndexBuffer()->getDataPtr();

		// create the buffer first
		glGenBuffers(1, &handle);
		CHECK_GL_ERROR;

		// bind the buffer and put the data into it
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes_per_entry * num_entries, data, GL_STATIC_DRAW);
		CHECK_GL_ERROR;
	}

	return;
}


void GlIndexBufferContent::releaseIndexBuffer() {
	if (handle) {
		glDeleteBuffers(1, &handle);
		handle = 0;
	}

	return;
}
