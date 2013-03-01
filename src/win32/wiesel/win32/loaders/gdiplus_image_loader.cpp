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
#include "gdiplus_image_loader.h"
#include "wiesel/resources/graphics/imageutils.h"

#include <wiesel/util/log.h>
#include <wiesel/module_registry.h>

#include <string>
#include <windows.h>


using namespace wiesel;
using namespace wiesel::win32;


GdiPlusImageLoader::GdiPlusImageLoader() : IImageLoader() {
	Gdiplus::GdiplusStartupInput startupInput;
	Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
	return;
}

GdiPlusImageLoader *GdiPlusImageLoader::create() {
	return new GdiPlusImageLoader();
}

GdiPlusImageLoader::~GdiPlusImageLoader() {
	Gdiplus::GdiplusShutdown(token);
	return;
}


Image *GdiPlusImageLoader::loadImage(DataSource *source) {
	return internal_loadImage(source, NULL, false);
}


Image *GdiPlusImageLoader::loadPowerOfTwoImage(DataSource *source, dimension *pOriginal_size) {
	return internal_loadImage(source, pOriginal_size, true);
}


Image *GdiPlusImageLoader::internal_loadImage(DataSource *source, dimension *pOriginalSize, bool pot) {
	Gdiplus::Bitmap *bitmap = NULL;
	Image *image = NULL;

	FileDataSource *filedata = dynamic_cast<FileDataSource*>(source);
	if (filedata) {
		std::string filename = filedata->getFile()->getNativePath();

		// get the string size & allocate space for the wchar string
		DWORD w_size = MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, NULL, 0);
		wchar_t *w_filename = new wchar_t[w_size];
		assert(w_filename);

		// convert the filename into wchar
		MultiByteToWideChar(CP_ACP, 0, filename.c_str(), -1, w_filename, w_size);

		bitmap = Gdiplus::Bitmap::FromFile(w_filename);

		delete[] w_filename;
	}

	// when loading from a file was not possible or has failed, try loading from a buffer
	if (bitmap == NULL) {
		DataBuffer *src_buffer = source->getDataBuffer();

		HRESULT hr;
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, src_buffer->getSize());
		assert(hMem);

		if (!hMem) {
			return NULL;
		}

		// copy image data into the buffer
		LPVOID pBufferData = ::GlobalLock(hMem);
		memcpy(pBufferData, src_buffer->getData(), src_buffer->getSize());
		::GlobalUnlock(hMem);

		IStream *stream = NULL;
		hr = ::CreateStreamOnHGlobal(hMem, FALSE, &stream);

		if (SUCCEEDED(hr)) {
			// Limit the stream to its real size
			ULARGE_INTEGER ulSize;
			ulSize.QuadPart = src_buffer->getSize();
			hr = stream->SetSize(ulSize);
			assert(SUCCEEDED(hr));

			// load the bitmap from the created stream
			bitmap = Gdiplus::Bitmap::FromStream(stream);
		}

		// release stream
		stream->Release();
		stream = NULL;

		// free HGLOBAL memory
		::GlobalFree(hMem);
	}

	// check if bitmap was loaded
	if (bitmap == NULL) {
		return NULL;
	}

	// get the image format
	Gdiplus::PixelFormat gdi_pixelformat = bitmap->GetPixelFormat();
	wiesel::PixelFormat wiesel_pixelformat = PixelFormat_Unknown;

	// get the gdi pixelformat
	switch(gdi_pixelformat) {
		case PixelFormat24bppRGB: {
			wiesel_pixelformat = PixelFormat_RGB_888;
			break;
		}

		case PixelFormat32bppARGB: {
			wiesel_pixelformat = PixelFormat_RGBA_8888;
			break;
		}

		default: {
			// when the pixel format is unknown or unsupported,
			// we force gdi to use either 24bit RGB or 32bit ARGB

			if (gdi_pixelformat & PixelFormatAlpha) {
				gdi_pixelformat    = PixelFormat32bppARGB;
				wiesel_pixelformat = PixelFormat_RGBA_8888;
			}
			else {
				gdi_pixelformat    = PixelFormat32bppARGB;
				wiesel_pixelformat = PixelFormat_RGBA_8888;
			}

			break;
		}
	}

	// when we have a valid image format, we can create our image object
	if (wiesel_pixelformat != PixelFormat_Unknown) {
		int bytesPerPixel = getBytesPerPixel(wiesel_pixelformat);

		// We must use LockBits to get the raw data. GetPixel and SetPixel are simply too slow
		Gdiplus::BitmapData bitmapData;
		Gdiplus::Rect bitmapSize(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
		bitmap->LockBits(
						&bitmapSize,
						Gdiplus::ImageLockModeRead,
						gdi_pixelformat,
						&bitmapData
		);

		uint32_t original_width  = bitmapData.Width;
		uint32_t original_height = bitmapData.Height;
		uint32_t width;
		uint32_t height;

		// when we're loading a texture, we may need a power-of-two size
		if (pot) {
			width  = getNextPowerOfTwo(original_width);
			height = getNextPowerOfTwo(original_height);
		}
		else {
			width  = original_width;
			height = original_height;
		}

		// allocate buffer
		DataBuffer *buffer = ExclusiveDataBuffer::create(width * height * bytesPerPixel);
		DataBuffer::mutable_data_t out_ptr = buffer->getMutableData();
		DataBuffer::data_t in_ptr = reinterpret_cast<DataBuffer::data_t>(bitmapData.Scan0);

		// copy image data into buffer
		int bytesPerRowSrc = original_width * bytesPerPixel;
		int bytesPerRowDst = width * bytesPerPixel;
		int gap            = bytesPerRowDst - bytesPerRowSrc;
		for(unsigned int row=0; row<original_height; ++row) {
			// copy the entire line
			memcpy(out_ptr + (row * bytesPerRowDst), in_ptr, bytesPerRowSrc);

			// fill the gap to the next line with transparent black color
			memset(out_ptr + (row * bytesPerRowDst) + bytesPerRowSrc, 0x00, gap);

			in_ptr += bitmapData.Stride;
		}

		// fill empty space of resized area
		memset(
				out_ptr + (original_height * bytesPerRowDst),
				0x00,
				(height - original_height) * bytesPerRowDst
		);

		// image conversion
		// TODO: this functionality needs to be moved into the image class
		switch(gdi_pixelformat) {
			case PixelFormat24bppRGB: {
				uint8_t *pPixel = reinterpret_cast<uint8_t*>(buffer->getMutableData());
				for(int i=width * height; --i>=0;) {
					uint8_t b = pPixel[0];
					uint8_t g = pPixel[1];
					uint8_t r = pPixel[2];

					pPixel[0] = r;
					pPixel[1] = g;
					pPixel[2] = b;

					pPixel += 3;
				}

				break;
			}

			case PixelFormat32bppARGB: {
				uint8_t *pPixel = reinterpret_cast<uint8_t*>(buffer->getMutableData());
				for(int i=width * height; --i>=0;) {
					uint8_t b = pPixel[0];
					uint8_t g = pPixel[1];
					uint8_t r = pPixel[2];
					uint8_t a = pPixel[3];

					pPixel[0] = r;
					pPixel[1] = g;
					pPixel[2] = b;
					pPixel[3] = a;

					pPixel += 4;
				}

				break;
			}
		}

		// create the image object
		image = new Image(
				buffer,
				wiesel_pixelformat,
				dimension(width, height)
		);

		// unlock the image
		bitmap->UnlockBits(&bitmapData);

		// store original size if requested
		if (pOriginalSize) {
			pOriginalSize->width  = original_width;
			pOriginalSize->height = original_height;
		}
	}

	// free the gdi image
	delete bitmap;

	return image;
}
