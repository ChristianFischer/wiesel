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
#include "gl_vertexbuffer_content.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;


GlVertexBufferContent::GlVertexBufferContent(VertexBuffer *vb) : VertexBufferContent(vb) {
	this->handle		= 0;
	return;
}

GlVertexBufferContent::~GlVertexBufferContent() {
	releaseVertexBuffer();
}



GlVertexBufferContent *GlVertexBufferContent::createContentFor(VertexBuffer* vb) {
	GlVertexBufferContent *gl_vb = new GlVertexBufferContent(vb);
	gl_vb->initializeVertexBuffer();
	return gl_vb;
}


void GlVertexBufferContent::initializeVertexBuffer() {
	if (handle == 0) {
		unsigned short				vertex_size		= getVertexBuffer()->getVertexSize();
		VertexBuffer::index_t		num_vertices	= getVertexBuffer()->getSize();
		const VertexBuffer::data_t	data			= getVertexBuffer()->getDataPtr();

		// create the buffer first
		glGenBuffers(1, &handle);
		CHECK_GL_ERROR;

		// bind the buffer and put the data into it
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, vertex_size * num_vertices, data, GL_STATIC_DRAW);
		CHECK_GL_ERROR;
	}

	return;
}


void GlVertexBufferContent::releaseVertexBuffer() {
	if (handle) {
		glDeleteBuffers(1, &handle);
		handle = 0;
	}

	return;
}

