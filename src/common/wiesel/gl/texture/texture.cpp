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
#include <wiesel.h>
#include <wiesel/util/imageutils.h>


using namespace wiesel;
using namespace std;


Texture::Texture() {
	this->handle = 0;
	return;
}

Texture::~Texture() {
	if (handle) {
		release_texture();
	}

	if (data) {
		data->release();
	}

	return;
}


Texture *Texture::fromFile(File *file) {
	return fromDataSource(file->asDataSource());
}


Texture *Texture::fromDataSource(DataSource *data) {
	Texture *texture = new Texture();
	texture->data = data;
	texture->data->retain();
	texture->createHardwareTexture();

	return texture;
}


bool Texture::createHardwareTexture() {
	assert(handle == 0);

	// release the previous buffer
	release_texture();

	unsigned char*	buffer		= NULL;
	size_t			buffer_size	= 0;
	unsigned int	width		= 0;
	unsigned int	height		= 0;
	unsigned int	orig_width	= 0;
	unsigned int	orig_height	= 0;
	int				r_bits		= 0;
	int				g_bits		= 0;
	int				b_bits		= 0;
	int				a_bits		= 0;

	bool successful = Engine::getCurrent()->decodeImage(
												data,
												&buffer, &buffer_size,
												&width, &height,
												&orig_width, &orig_height,
												&r_bits, &g_bits, &b_bits, &a_bits,
												true
	);

	// compute the byte-size of the image
	int bytesPerPixel = (r_bits + g_bits + b_bits + a_bits) / 8;

	unsigned int texture_width	= width;
	unsigned int texture_height	= height;

	if (successful && buffer) {
		// when power-of-two sizes are required, compute the next pot-size of the texture
		texture_width	= imageutils::getNextPowerOfTwo(texture_width);
		texture_height	= imageutils::getNextPowerOfTwo(texture_height);

		if ((texture_width != width) || (texture_height != height)) {
			unsigned char *new_buffer = imageutils::resizeImage(
											buffer, bytesPerPixel,
											width,			height,
											texture_width,	texture_height
			);

			// something went wrong - so delete the current buffer
			if (new_buffer == NULL) {
				successful = false;
				free(buffer);
			}

			buffer = new_buffer;
		}
	}

	if (successful && buffer) {
		// create the hardware texture
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		size          = dimension(texture_width, texture_height);
		original_size = dimension(orig_width,    orig_height);
	}

	if (buffer) {
		free(buffer);
	}

	return successful;
}


void Texture::release_texture() {
	if (handle) {
		glDeleteTextures(1, &handle);
		handle = 0;
	}

	return;
}
