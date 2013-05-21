/**
 * Copyright (C) 2013
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
#include "rect_shape_node.h"

#include <wiesel/video/render_context.h>
#include <wiesel/video/shaders.h>


#define VERTEX_INDEX_TL		0
#define VERTEX_INDEX_BL		1
#define VERTEX_INDEX_TR		2
#define VERTEX_INDEX_BR		3


using namespace wiesel;
using namespace wiesel::video;


RectShapeNode::RectShapeNode() {
}

RectShapeNode::RectShapeNode(const rectangle& rect) {
	setRect(rect);
}

RectShapeNode::RectShapeNode(const dimension& size) {
	setRect(size);
}

RectShapeNode::RectShapeNode(const vector2d& position, const dimension& size) {
	setRect(position, size);
}

RectShapeNode::RectShapeNode(float w, float h) {
	setRect(w, h);
}

RectShapeNode::RectShapeNode(float x, float y, float w, float h) {
	setRect(x, y, w, h);
}


RectShapeNode::~RectShapeNode() {
}



void RectShapeNode::setRect(const rectangle& rect) {
	setRect(rect.position.x, rect.position.y, rect.size.width, rect.size.height);
}

void RectShapeNode::setRect(const dimension& size) {
	setRect(0, 0, size.width, size.height);
}

void RectShapeNode::setRect(const vector2d& position, const dimension& size) {
	setRect(position.x, position.y, size.width, size.height);
}

void RectShapeNode::setRect(float w, float h) {
	setRect(0, 0, w, h);
}

void RectShapeNode::setRect(float x, float y, float w, float h) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexPosition(VERTEX_INDEX_TL, x,   y+h);
	vbo->setVertexPosition(VERTEX_INDEX_BL, x,   y  );
	vbo->setVertexPosition(VERTEX_INDEX_TR, x+w, y+h);
	vbo->setVertexPosition(VERTEX_INDEX_BR, x+w, y  );

	setBounds(rectangle(x, y, w, h));

	return;
}



void RectShapeNode::setColor(float r, float g, float b, float a) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexColor(VERTEX_INDEX_TL, r, g, b, a);
	vbo->setVertexColor(VERTEX_INDEX_BL, r, g, b, a);
	vbo->setVertexColor(VERTEX_INDEX_TR, r, g, b, a);
	vbo->setVertexColor(VERTEX_INDEX_BR, r, g, b, a);

	return;
}


void RectShapeNode::setColorTopLeft(float r, float g, float b, float a) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexColor(VERTEX_INDEX_TL, r, g, b, a);

	return;
}


void RectShapeNode::setColorTopRight(float r, float g, float b, float a) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexColor(VERTEX_INDEX_TR, r, g, b, a);

	return;
}

void RectShapeNode::setColorBottomLeft(float r, float g, float b, float a) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexColor(VERTEX_INDEX_BL, r, g, b, a);

	return;
}


void RectShapeNode::setColorBottomRight(float r, float g, float b, float a) {
	ensureVertexBufferExists();

	assert(vbo);
	assert(vbo->getSize() == 4);

	vbo->setVertexColor(VERTEX_INDEX_BR, r, g, b, a);

	return;
}



void RectShapeNode::onDraw(RenderContext* render_context) {
	if (vbo) {
		this->applyShaderConfigTo(render_context);
		render_context->setModelviewMatrix(getWorldTransform());
		render_context->draw(video::TriangleStrip, vbo);
	}

	return;
}



void RectShapeNode::ensureVertexBufferExists() {
	if (vbo == NULL) {
		vbo = new VertexBuffer();
		vbo->setupVertexPositions(2);
		vbo->setupVertexColors(4);

		vbo->setCapacity(4);
		vbo->addVertex(vector2d::zero);
		vbo->addVertex(vector2d::zero);
		vbo->addVertex(vector2d::zero);
		vbo->addVertex(vector2d::zero);
	}

	assert(vbo);
	assert(vbo->getSize() == 4);

	if (getShader() == NULL) {
		setShader(Shaders::instance()->getShaderFor(vbo));
	}

	return;
}

