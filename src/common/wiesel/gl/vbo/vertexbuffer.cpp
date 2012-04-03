/*
 * vertexbuffer.cpp
 *
 *  Created on: 30.03.2012
 *      Author: Baldur
 */

#include "vertexbuffer.h"

#include <wiesel/platform/android/log.h>

#include <assert.h>


using namespace wiesel;


VertexBuffer::VertexBuffer() {
	return;
}

VertexBuffer::~VertexBuffer() {
	return;
}


VertexBuffer::index_t VertexBuffer::addVertex(float x, float y) {
	index_t index = data.size();

	data.push_back(x);
	data.push_back(y);
	data.push_back(0.0f);

	return index;
}


VertexBuffer::index_t VertexBuffer::addVertex(float x, float y, float z) {
	index_t index = data.size();

	data.push_back(x);
	data.push_back(y);
	data.push_back(z);

	return index;
}


bool VertexBuffer::bind(const ShaderProgram *program) {
	assert(program);
	bool success = true;

	// pointer to the vertex data
	void *pData = data.data();

	// bind the shader
	program->bind();

	GLuint attr_vertex_position = program->getVertexPositionAttribute();
	assert(attr_vertex_position != -1);
	if (attr_vertex_position != -1) {
	    glVertexAttribPointer(attr_vertex_position, 3, GL_FLOAT, GL_FALSE, 0, pData);
		CHECK_GL_ERROR;

	    glEnableVertexAttribArray(attr_vertex_position);
		CHECK_GL_ERROR;
	}
	else {
		success = false;
	}

	return success;
}


void VertexBuffer::render() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
	CHECK_GL_ERROR;
	return;
}

