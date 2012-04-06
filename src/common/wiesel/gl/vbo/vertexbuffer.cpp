/*
 * vertexbuffer.cpp
 *
 *  Created on: 30.03.2012
 *      Author: Baldur
 */

#include "vertexbuffer.h"
#include <assert.h>


using namespace wiesel;


VertexBuffer::VertexBuffer() {
	this->num_vertices	= 0;
	this->capacity		= 0;
	this->data			= NULL;

	setupVertexPositions(3);
	disableVertexNormals();
	disableVertexColors();

	return;
}

VertexBuffer::~VertexBuffer() {
	clear();

	if (data) {
		free(data);
	}

	return;
}



bool VertexBuffer::checkIfSetupPossible() const {
	assert(capacity == 0);
	if (capacity != 0) {
		return false;
	}

	return true;
}



bool VertexBuffer::setupVertexPositions(int dimensions) {
	if (checkIfSetupPossible()) {
		assert(dimensions == 2 || dimensions == 3);

		if (dimensions != 2 && dimensions != 3) {
			return false;
		}

		positions.fields = dimensions;
		positions.size   = dimensions * sizeof(float);

		updateOffsets();

		return true;
	}

	return false;
}


bool VertexBuffer::setupVertexNormals() {
	if (checkIfSetupPossible()) {
		normals.fields = 3;
		normals.size   = 3 * sizeof(float);
		updateOffsets();
		return true;
	}

	return false;
}


bool VertexBuffer::setupVertexColors(int fields) {
	if (checkIfSetupPossible()) {
		assert(fields == 3 || fields == 4);

		if (fields != 3 && fields != 4) {
			return false;
		}

		colors.fields = fields;
		colors.size   = fields * sizeof(float);

		updateOffsets();

		return true;
	}

	return false;
}


bool VertexBuffer::setupTextureLayer(int layer) {
	if (checkIfSetupPossible()) {
		assert(layer <= textures.size());
		assert(layer >= 0);

		// illegal texture layer.
		if (layer > textures.size()) {
			return false;
		}

		component texture;
		texture.fields = 2;
		texture.size   = 2 * sizeof(float);

		if (layer == textures.size()) {
			textures.push_back(texture);
		}
		else {
			textures[layer] = texture;
		}

		updateOffsets();

		return true;
	}

	return false;
}



void VertexBuffer::disableVertexNormals() {
	if (checkIfSetupPossible()) {
		normals.size = 0;
		updateOffsets();
	}

	return;
}


void VertexBuffer::disableVertexColors() {
	if (checkIfSetupPossible()) {
		colors.size = 0;
		updateOffsets();
	}

	return;
}


void VertexBuffer::disableTextureLayer(int layer) {
	if (checkIfSetupPossible()) {
		// it's only possible to delete the last texture layer
		assert(layer == textures.size() - 1);

		if (layer == textures.size() - 1) {
			textures.erase(textures.begin() + textures.size() - 1);
		}

		updateOffsets();
	}

	return;
}


void VertexBuffer::updateOffsets() {
	vertex_size = 0;

	positions.offset = vertex_size;
	vertex_size += positions.size;

	normals.offset = vertex_size;
	vertex_size += normals.size;

	colors.offset = vertex_size;
	vertex_size += colors.size;

	for(std::vector<component>::iterator it=textures.begin(); it!=textures.end(); it++) {
		it->offset = vertex_size;
		vertex_size += it->size;
	}

	return;
}


VertexBuffer::data_t VertexBuffer::getVertexPtr(index_t index, const component &comp) {
	assert(index < num_vertices);

	if (index < num_vertices) {
		data_t ptr = (data + (vertex_size * index) + comp.offset);
		return ptr;
	}

	return NULL;
}






VertexBuffer::index_t VertexBuffer::setCapacity(index_t capacity) {
	if (this->data == NULL) {
		this->data = reinterpret_cast<data_t>(malloc(vertex_size * capacity));

		// clear the allocated memory
		memset(this->data, NULL, vertex_size * capacity);
		assert(this->data);

		if (this->data) {
			this->capacity = capacity;
		}
		else {
			this->capacity = 0;
		}
	}
	else {
		// no not change the capacity, if the new capacity is
		// smaller than the current number of vertices
		assert(capacity >= num_vertices);
		if (capacity < num_vertices) {
			return this->capacity;
		}

		data_t new_data = reinterpret_cast<data_t>(realloc(data, vertex_size * capacity));
		assert(new_data);

		if (new_data) {
			if (data != new_data) {
				data  = new_data;
			}

			this->capacity = capacity;
		}
	}

	return this->capacity;
}


VertexBuffer::index_t VertexBuffer::ensureCapacity(index_t capacity) {
	if (this->capacity < capacity) {
		return setCapacity(capacity);
	}

	return this->capacity;
}


VertexBuffer::index_t VertexBuffer::getCapacity() const {
	return this->capacity;
}


VertexBuffer::index_t VertexBuffer::getSize() const {
	return this->num_vertices;
}


void VertexBuffer::clear() {
	if (data) {
		free(data);
		data = NULL;
	}

	capacity     = 0;
	num_vertices = 0;

	return;
}





VertexBuffer::index_t VertexBuffer::addVertex(float x, float y, float z) {
	index_t index = num_vertices;
	index_t new_capacity = ensureCapacity(index + 1);

	if (new_capacity > index) {
		num_vertices++;
		setVertexPosition(index, x, y, z);
	}

	return index;
}


void VertexBuffer::setVertexPosition(index_t index, float x, float y, float z) {
	data_t ptr = getVertexPtr(index, positions);
	int field_size = sizeof(float);
	assert(ptr);

	if (ptr) {
		if (positions.fields >= 1) {
			setValue(ptr, field_size * 0, x);
		}

		if (positions.fields >= 2) {
			setValue(ptr, field_size * 1, y);
		}

		if (positions.fields >= 3) {
			setValue(ptr, field_size * 2, z);
		}
	}

	return;
}


void VertexBuffer::setVertexNormal(index_t index, float x, float y, float z) {
	data_t ptr = getVertexPtr(index, normals);
	int field_size = sizeof(float);
	assert(ptr);

	if (ptr) {
		if (normals.fields >= 1) {
			setValue(ptr, field_size * 0, x);
		}

		if (normals.fields >= 2) {
			setValue(ptr, field_size * 1, y);
		}

		if (normals.fields >= 3) {
			setValue(ptr, field_size * 2, z);
		}
	}

	return;
}


void VertexBuffer::setVertexColor(index_t index, float r, float g, float b, float a) {
	data_t ptr = getVertexPtr(index, colors);
	int field_size = sizeof(float);
	assert(ptr);

	if (ptr) {
		if (colors.fields >= 1) {
			setValue(ptr, field_size * 0, r);
		}

		if (colors.fields >= 2) {
			setValue(ptr, field_size * 1, g);
		}

		if (colors.fields >= 3) {
			setValue(ptr, field_size * 2, b);
		}

		if (colors.fields >= 4) {
			setValue(ptr, field_size * 3, a);
		}
	}

	return;
}


void VertexBuffer::setVertexTextureCoordinate(index_t index, int layer, float u, float v) {
	assert(layer < textures.size());
	if (layer < textures.size()) {
		data_t ptr = getVertexPtr(index, textures[layer]);
		int field_size = sizeof(float);
		assert(ptr);

		if (ptr) {
			if (colors.fields >= 1) {
				setValue(ptr, field_size * 0, u);
			}

			if (colors.fields >= 2) {
				setValue(ptr, field_size * 1, v);
			}
		}
	}

	return;
}





bool VertexBuffer::bind(const ShaderProgram *program) {
	assert(program);
	bool success = true;

	// bind the shader
	program->bind();

	if (positions.size) {
		GLuint attr_vertex_position = program->getVertexPositionAttribute();
		assert(attr_vertex_position != -1);
		if (attr_vertex_position != -1) {
			glVertexAttribPointer(attr_vertex_position, positions.fields, GL_FLOAT, GL_FALSE, vertex_size, data + positions.offset);
			glEnableVertexAttribArray(attr_vertex_position);
			CHECK_GL_ERROR;
		}
		else {
			success = false;
		}
	}

	if (normals.size) {
		GLuint attr_vertex_normals = program->getVertexNormalAttribute();
		assert(attr_vertex_normals != -1);
		if (attr_vertex_normals != -1) {
			glVertexAttribPointer(attr_vertex_normals, normals.fields, GL_FLOAT, GL_FALSE, vertex_size, data + normals.offset);
			glEnableVertexAttribArray(attr_vertex_normals);
			CHECK_GL_ERROR;
		}
	}

	if (colors.size) {
		GLuint attr_vertex_colors = program->getVertexColorAttribute();
		assert(attr_vertex_colors != -1);
		if (attr_vertex_colors != -1) {
			glVertexAttribPointer(attr_vertex_colors, colors.fields, GL_FLOAT, GL_FALSE, vertex_size, data + colors.offset);
			glEnableVertexAttribArray(attr_vertex_colors);
			CHECK_GL_ERROR;
		}
	}

	return success;
}


void VertexBuffer::unbind(const ShaderProgram *program) {
	if (positions.size) {
		GLuint attr_vertex_position = program->getVertexPositionAttribute();
		glDisableVertexAttribArray(attr_vertex_position);
	}

	if (normals.size) {
		GLuint attr_vertex_normals = program->getVertexNormalAttribute();
		glDisableVertexAttribArray(attr_vertex_normals);
	}

	if (colors.size) {
		GLuint attr_vertex_colors = program->getVertexColorAttribute();
		glDisableVertexAttribArray(attr_vertex_colors);
	}

	program->unbind();

	return;
}


void VertexBuffer::render() {
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CHECK_GL_ERROR;
	return;
}
