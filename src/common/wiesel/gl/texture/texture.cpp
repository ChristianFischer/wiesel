/*
 * texture.cpp
 *
 *  Created on: 07.04.2012
 *      Author: Baldur
 */

#include "texture.h"
#include <wiesel.h>


using namespace wiesel;
using namespace std;


Texture::Texture() {
	this->handle = 0;
	return;
}

Texture::~Texture() {
	// texture should be released before deleting the object
	assert(handle == 0);

	if (handle) {
		release();
	}

	return;
}


Texture *Texture::fromFile(const string &filename) {
	Texture *texture = new Texture();
	texture->filename = filename;
	texture->createHardwareTexture();

	return texture;
}


bool Texture::createHardwareTexture() {
	assert(handle == 0);

	// release the previous buffer
	release();

	unsigned char*	buffer		= NULL;
	size_t			buffer_size	= 0;
	unsigned int	width		= 0;
	unsigned int	height		= 0;
	int				r_bits		= 0;
	int				g_bits		= 0;
	int				b_bits		= 0;
	int				a_bits		= 0;

	bool successful = Engine::getCurrent()->decodeImage(
												filename,
												&buffer, &buffer_size, &width, &height,
												&r_bits, &g_bits, &b_bits, &a_bits,
												true
	);

	if (successful && buffer) {
		// create the hardware texture
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		size = dimension(width, height);
	}

	if (buffer) {
		free(buffer);
	}

	return successful;
}


void Texture::release() {
	if (handle) {
		glDeleteTextures(1, &handle);
		handle = 0;
	}

	return;
}
