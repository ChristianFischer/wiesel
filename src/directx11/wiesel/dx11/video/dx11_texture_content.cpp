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
#include "dx11_texture_content.h"
#include "dx11_render_context.h"

#include <wiesel.h>
#include <wiesel/resources/graphics/image.h>
#include <wiesel/resources/graphics/imageutils.h>
#include <wiesel/resources/graphics/image_loader.h>
#include <wiesel/module_registry.h>


using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


Dx11TextureContent::Dx11TextureContent(Texture *texture) : TextureContent(texture) {
	this->texture				= NULL;
	this->shader_resource_view	= NULL;
	this->sampler_state			= NULL;
	return;
}

Dx11TextureContent::~Dx11TextureContent() {
	if (texture) {
		releaseTexture();
	}

	return;
}


Dx11TextureContent *Dx11TextureContent::createContentFor(DirectX11RenderContext *context, Texture *texture) {
	Dx11TextureContent *dx11_texture = new Dx11TextureContent(texture);

	if (dx11_texture->initializeTexture(context) == false) {
		return NULL;
	}
	
	return dx11_texture;
}


bool Dx11TextureContent::initializeTexture(DirectX11RenderContext *context) {
	assert(texture == NULL);

	// release the previous texture
	releaseTexture();

	DataSource *data = getTexture()->getSource();
	Image *image = NULL;
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

	// store the image's size
	size          = image->getSize();
	original_size = new_original_size;

	size_t bytesPerPixel = getBytesPerPixel(image->getPixelFormat());
	D3D11_TEXTURE2D_DESC   texture_desc;
	D3D11_SUBRESOURCE_DATA subresource_data;
	HRESULT result;

	// initialize source data
	subresource_data.pSysMem			= image->getPixelData()->getData();
	subresource_data.SysMemPitch		= image->getSize().width * bytesPerPixel;
	subresource_data.SysMemSlicePitch	= image->getSize().width * image->getSize().height * bytesPerPixel;

	texture_desc.Width					= image->getSize().width;
	texture_desc.Height					= image->getSize().height;
	texture_desc.ArraySize				= 1;
	texture_desc.MipLevels				= 1;
	texture_desc.SampleDesc.Count		= 1;
	texture_desc.SampleDesc.Quality		= 0;
	texture_desc.Usage					= D3D11_USAGE_DEFAULT;
	texture_desc.Format					= DXGI_FORMAT_UNKNOWN;
	texture_desc.BindFlags				= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags			= 0;
	texture_desc.MiscFlags				= 0;

	switch(image->getPixelFormat()) {
		case PixelFormat_RGBA_8888: {
			texture_desc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		}

		case PixelFormat_RGB_888: {
			// TODO: WTF?? 24bit formats were dropped? oO
			break;
		}

		case PixelFormat_Unknown: {
			texture_desc.Format			= DXGI_FORMAT_UNKNOWN;
			return false;
		}
	}

	if (texture_desc.Format == DXGI_FORMAT_UNKNOWN) {
		return false;
	}

	// create the hardware texture
	result = context->getD3DDevice()->CreateTexture2D(
											&texture_desc,
											&subresource_data,
											&texture
	);

	if (FAILED(result)) {
		return false;
	}

	// fetch the texture desciption after loading
	texture->GetDesc(&texture_desc);

	// setup the resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc;
	resource_view_desc.Format						= texture_desc.Format;
	resource_view_desc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_view_desc.Texture2D.MipLevels			= texture_desc.MipLevels;
	resource_view_desc.Texture2D.MostDetailedMip	= 0;

	// create the shader resource
	result = context->getD3DDevice()->CreateShaderResourceView(
											texture,
											&resource_view_desc,
											&shader_resource_view
	);

	if (FAILED(result)) {
		return false;
	}

	// create a texture sampler state description.
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter				= D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias			= 0.0f;
	sampler_desc.MaxAnisotropy		= 1;
	sampler_desc.ComparisonFunc		= D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0]		= 0;
	sampler_desc.BorderColor[1]		= 0;
	sampler_desc.BorderColor[2]		= 0;
	sampler_desc.BorderColor[3]		= 0;
	sampler_desc.MinLOD				= 0;
	sampler_desc.MaxLOD				= D3D11_FLOAT32_MAX;

	result = context->getD3DDevice()->CreateSamplerState(&sampler_desc, &sampler_state);

	if (FAILED(result)) {
		return false;
	}

	return true;
}


void Dx11TextureContent::releaseTexture() {
	if (shader_resource_view) {
		shader_resource_view->Release();
		shader_resource_view = NULL;
	}

	if (sampler_state) {
		sampler_state->Release();
		sampler_state = NULL;
	}

	if (texture) {
		texture->Release();
		texture = NULL;
	}

	return;
}
