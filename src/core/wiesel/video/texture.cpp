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
#include "texture.h"
#include "video_driver.h"

#include <wiesel.h>
#include <wiesel/resources/graphics/image.h>
#include <wiesel/resources/graphics/imageutils.h>
#include <wiesel/resources/graphics/image_loader.h>
#include <wiesel/module_registry.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace std;


Texture::Texture() {
	data = NULL;
	return;
}

Texture::~Texture() {
	safe_release(data);
	return;
}


Texture *Texture::fromFile(File *file) {
	return fromDataSource(file->asDataSource());
}


Texture *Texture::fromDataSource(DataSource *data) {
	Texture *texture = new Texture();
	texture->data = data;
	keep(texture->data);

	return texture;
}


Texture *Texture::createEmptyTexture(const dimension& size) {
	Texture *texture = new Texture();
	texture->requested_size = size;
	return texture;
}



bool Texture::doLoadContent() {
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

	TextureContent *rc = driver->createTextureContent(this);
	if (rc == NULL) {
		return false;
	}

	// preserve the texture's dimensions
	size          = rc->getSize();
	original_size = rc->getOriginalSize();

	setContent(rc);

	return true;
}


bool Texture::doUnloadContent() {
	setContent(NULL);
	return true;
}




TextureContent::TextureContent() {
	return;
}

TextureContent::TextureContent(Texture *texture) {
	this->texture = texture;
	return;
}

TextureContent::~TextureContent() {
	return;
}
