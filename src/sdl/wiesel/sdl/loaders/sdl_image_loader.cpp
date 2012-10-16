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
#include "sdl_image_loader.h"

#include <wiesel/util/log.h>

#include <SDL_image.h>
#include <string>


using namespace wiesel;
using namespace std;


SdlImageLoader::SdlImageLoader() : IImageLoader() {
	int initialized = IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	if((initialized & IMG_INIT_PNG) == 0) {
		Log::err << "Loading libPNG failed." << std::endl;
	}
	if((initialized & IMG_INIT_JPG) == 0) {
		Log::err << "Loading libJPG failed." << std::endl;
	}

	return;
}

SdlImageLoader *SdlImageLoader::create() {
	return new SdlImageLoader();
}

SdlImageLoader::~SdlImageLoader() {
	IMG_Quit();
	return;
}


Image *SdlImageLoader::loadImage(DataSource *source) {
	return internal_loadImage(source, NULL, false);
}


Image *SdlImageLoader::loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size) {
	return internal_loadImage(source, pOriginal_size, true);
}


Image *SdlImageLoader::internal_loadImage(DataSource *source, dimension *pOriginalSize, bool pot) {
	SDL_Surface *surface = NULL;

	FileDataSource *fds = dynamic_cast<FileDataSource*>(source);
	if (fds) {
		File *file = fds->getFile();
		string path = file->getNativePath();

		if (path.empty() == false) {
			surface = IMG_Load(path.c_str());
		}
	}

	// when loading from file failed, try loading via buffer
	if (surface == NULL) {
		DataBuffer *buffer = source->getDataBuffer();
		SDL_RWops *rw = SDL_RWFromConstMem(buffer->getData(), buffer->getSize());
		surface = IMG_Load_RW(rw, 0);
		SDL_FreeRW(rw);
	}

	if (surface) {
		PixelFormat pixelformat;

		switch(surface->format->BytesPerPixel) {
			case 4: {
				pixelformat = PixelFormat_RGBA_8888;
				break;
			}

			case 3: {
				pixelformat = PixelFormat_RGB_888;
				break;
			}

			default: {
				// no compatible pixel format
				SDL_FreeSurface(surface);
				return NULL;
			}
		}

		// compute the buffer size
		size_t image_size = surface->w * surface->h * surface->format->BytesPerPixel;

		// copy the pixel data into the buffer
		DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(surface->pixels, image_size);
		if (buffer == NULL) {
			return NULL;
		}

		// create the image object
		Image *image = new Image(buffer, pixelformat, dimension(surface->w, surface->h));

		// set results
		if (pOriginalSize) {
			pOriginalSize->width  = surface->w;
			pOriginalSize->height = surface->h;
		}

		// check power-of-two size
		if (pot) {
			image->ensurePowerOfTwo();
		}

		// release the surface
		SDL_FreeSurface(surface);

		return image;
	}

	return NULL;
}



// add the module to the module registry
REGISTER_MODULE_SINGLETON(
		IImageLoader,
		SdlImageLoader,
		&SdlImageLoader::create,
		"SDL",
		0x01020000u,
		IModuleLoader::PriorityNormal
)
