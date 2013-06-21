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
#include "gl_texture_content.h"

#include <wiesel.h>
#include <wiesel/resources/graphics/image.h>
#include <wiesel/resources/graphics/imageutils.h>
#include <wiesel/resources/graphics/image_loader.h>
#include <wiesel/module_registry.h>

#include <string.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;


GlTextureContent::GlTextureContent(Texture *texture) : TextureContent(texture) {
	this->handle = 0;
	return;
}

GlTextureContent::~GlTextureContent() {
	if (handle) {
		releaseTexture();
	}

	return;
}


GlTextureContent *GlTextureContent::createContentFor(Texture *texture) {
	GlTextureContent *gl_texture = new GlTextureContent(texture);

	if (gl_texture->initTexture() == false) {
		delete gl_texture;

		return NULL;
	}
	
	return gl_texture;
}


bool GlTextureContent::initTexture() {
	assert(handle == 0);

	// release the previous buffer
	releaseTexture();

	if (getTexture()->getSource()) {
		return loadTextureFromSource(getTexture()->getSource());
	}

	if (getTexture()->getRequestedSize().getMin() > 0.0f) {
		return loadEmptyTexture(PixelFormat_RGBA_8888, getTexture()->getRequestedSize());
	}

	return false;
}


bool GlTextureContent::loadEmptyTexture(PixelFormat format, const dimension& size) {
	dimension new_size = size;

	// convert into power-of-two
	new_size.width  = getNextPowerOfTwo(static_cast<unsigned int>(size.width));
	new_size.height = getNextPowerOfTwo(static_cast<unsigned int>(size.height));

	size_t data_size = new_size.width * new_size.height * getBytesPerPixel(format);

	// create an empty buffer containing the initial texture data
	ref<DataBuffer> data = ExclusiveDataBuffer::create(data_size);
	if (data->getData() == NULL) {
		return false;
	}

	memset(data->getMutableData(), 0, data_size);

	if (!createHardwareTexture(PixelFormat_RGBA_8888, new_size, data)) {
		return false;
	}

	this->original_size = size;

	return true;
}


bool GlTextureContent::loadTextureFromSource(DataSource *data) {
	ref<Image> image = NULL;
	dimension new_original_size;

	std::vector<ModuleLoader<IImageLoader>*> loaders = ModuleRegistry::getInstance()->findModules<IImageLoader>();
	for(std::vector<ModuleLoader<IImageLoader>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		ref<IImageLoader> loader = (*it)->create();
		if (loader == NULL) {
			continue;
		}

		image = loader->loadPowerOfTwoImage(data, &new_original_size);
		if (image == NULL) {
			continue;
		}

		break;
	}

	// check if loading was successful
	if (image == NULL) {
		return false;
	}

	// ensure power-of-two size
	if (image->ensurePowerOfTwo() == false) {
		return false;
	}
	
	if (!createHardwareTexture(image->getPixelFormat(), image->getSize(), image->getPixelData())) {
		return false;
	}

	original_size = new_original_size;
	
	return true;
}


bool GlTextureContent::createHardwareTexture(PixelFormat format, const dimension& size, DataBuffer *data) {
	GLint internalFormat;
	GLenum image_format;
	GLenum image_type;

	switch(format) {
		case PixelFormat_RGBA_8888: {
			internalFormat = GL_RGBA;
			image_format   = GL_RGBA;
			image_type     = GL_UNSIGNED_BYTE;
			break;
		}

		case PixelFormat_RGB_888: {
			internalFormat = GL_RGB;
			image_format   = GL_RGB;
			image_type     = GL_UNSIGNED_BYTE;
			break;
		}

		case PixelFormat_Unknown: {
			return false;
		}
	}

	// create the hardware texture
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(
					GL_TEXTURE_2D, 0,
					internalFormat,
					size.width, size.height,
					0,
					image_format, image_type,
					data->getData()
	);

	this->size = size;

	return true;
}


void GlTextureContent::releaseTexture() {
	if (handle) {
		glDeleteTextures(1, &handle);
		handle = 0;
	}

	return;
}
