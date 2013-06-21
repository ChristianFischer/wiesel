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
#include "postprocessing_node.h"
#include "wiesel/video/render_context.h"
#include "wiesel/video/shaders.h"
#include "wiesel/video/video_driver.h"

using namespace wiesel;


PostProcessingNode::PostProcessingNode() {
}


PostProcessingNode::PostProcessingNode(video::RenderBuffer* render_buffer)
	: RenderBufferNode(render_buffer) {
	return;
}


PostProcessingNode::PostProcessingNode(video::Texture* target_texture) 
	: RenderBufferNode(target_texture) {
	return;
}


PostProcessingNode::~PostProcessingNode() {
}


void PostProcessingNode::render(video::RenderContext* render_context) {
	RenderBufferNode::render(render_context);
	this->renderBufferContent(render_context);
	return;
}


void PostProcessingNode::renderBufferContent(video::RenderContext* render_context) {
	if (getRenderBuffer() && vbo) {
		this->applyShaderConfigTo(render_context);
		render_context->setModelviewMatrix(getWorldTransform());
		render_context->prepareTextureLayers(getRenderBuffer()->getTargetTextureCount());

		for(int layer=getRenderBuffer()->getTargetTextureCount(); --layer>=0;) {
			render_context->setTexture(layer, getRenderBuffer()->getTargetTexture(layer));
		}

		// get the position of the screen corners in node-space
		video::VideoDeviceDriver *driver = render_context->getScreen()->getVideoDeviceDriver();
		dimension screensize = driver->getResolution();
		vector2d bottom_left = vector2d(0.0f, screensize.height);
		vector2d top_right   = vector2d(screensize.width, 0.0f);
		bottom_left          = convertWorldToLocal(driver->convertScreenToWorld(bottom_left));
		top_right            = convertWorldToLocal(driver->convertScreenToWorld(top_right));

		vbo->setVertexPosition(0, bottom_left.x, top_right.y);
		vbo->setVertexPosition(1, bottom_left.x, bottom_left.y);
		vbo->setVertexPosition(2, top_right.x,   top_right.y);
		vbo->setVertexPosition(3, top_right.x,   bottom_left.y);

		render_context->draw(video::TriangleStrip, vbo);
	}

	return;
}


void PostProcessingNode::onViewportChanged(const rectangle& old_viewport, const rectangle& new_viewport) {
	RenderBufferNode::onViewportChanged(old_viewport, new_viewport);

	updateVertexBuffer(new_viewport);

	return;
}


void PostProcessingNode::updateVertexBuffer(const rectangle& size) {
	if (vbo == NULL) {
		vbo = new video::VertexBuffer();
	}

	size_t num_textures = getRenderBuffer()->getTargetTextureCount();

	vbo->clear();
	vbo->setupVertexPositions(2);

	if (num_textures > 0) {
		for(unsigned int layer=0; layer<num_textures; layer++) {
			vbo->setupTextureLayer(layer);
		}
	}
	else {
		for(int layer=vbo->getNumberOfTextureLayers(); --layer>=0;) {
			vbo->disableTextureLayer(layer);
		}
	}

	vbo->addVertex(size.getMinX(), size.getMaxY());
	vbo->addVertex(size.getMinX(), size.getMinY());
	vbo->addVertex(size.getMaxX(), size.getMaxY());
	vbo->addVertex(size.getMaxX(), size.getMinY());

	for(int layer=num_textures; --layer>=0;) {
		video::Texture *texture = getRenderBuffer()->getTargetTexture(layer);

		if (texture && texture->getSize().getMin() > 0) {
			const dimension& texture_size = texture->getSize();

			float left   = size.getMinX() / texture_size.width;
			float right  = size.getMaxX() / texture_size.width;
			float top    = size.getMaxY() / texture_size.height;
			float bottom = size.getMinY() / texture_size.height;

			vbo->setVertexTextureCoordinate(0, layer, left,  top);
			vbo->setVertexTextureCoordinate(1, layer, left,  bottom);
			vbo->setVertexTextureCoordinate(2, layer, right, top);
			vbo->setVertexTextureCoordinate(3, layer, right, bottom);
		}
		else {
			vbo->setVertexTextureCoordinate(0, layer, 0.0f, 0.0f);
			vbo->setVertexTextureCoordinate(1, layer, 0.0f, 0.0f);
			vbo->setVertexTextureCoordinate(2, layer, 0.0f, 0.0f);
			vbo->setVertexTextureCoordinate(3, layer, 0.0f, 0.0f);
		}
	}

	if (getShader() == NULL) {
		setShader(video::Shaders::instance()->getShaderFor(vbo));
	}

	return;
}
