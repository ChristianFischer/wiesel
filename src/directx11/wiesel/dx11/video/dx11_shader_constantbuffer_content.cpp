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
#include "dx11_shader_constantbuffer_content.h"
#include "dx11_render_context.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


Dx11ShaderConstantBufferContent::Dx11ShaderConstantBufferContent(ShaderConstantBuffer *constant_buffer)
		: ShaderConstantBufferContent(constant_buffer)
{
	buffer			= NULL;
	last_version	= 0;
	return;
}

Dx11ShaderConstantBufferContent::~Dx11ShaderConstantBufferContent() {
	releaseConstantBuffer();
	return;
}

Dx11ShaderConstantBufferContent* Dx11ShaderConstantBufferContent::createContentFor(DirectX11RenderContext *context, ShaderConstantBuffer *constant_buffer) {
	Dx11ShaderConstantBufferContent *dx11_buffer = new Dx11ShaderConstantBufferContent(constant_buffer);

	if (dx11_buffer->initializeConstantBuffer(context) == false) {
		delete dx11_buffer;
		return NULL;
	}

	return dx11_buffer;
}


bool Dx11ShaderConstantBufferContent::initializeConstantBuffer(DirectX11RenderContext *context) {
	assert(buffer == NULL);

	// size needs to be a multiple of 16
	UINT buffer_size = getShaderConstantBuffer()->getTemplate()->getSize();
	buffer_size += (16 - (buffer_size % 16)) % 16;

	// size is required to be at least 128bytes?
	if (buffer_size < 128) {
		buffer_size = 128;
	}

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth				= buffer_size;
	buffer_desc.Usage					= D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags				= 0;
	buffer_desc.StructureByteStride		= 0;

	// create the constant buffer
	HRESULT result = context->getD3DDevice()->CreateBuffer(&buffer_desc, NULL, &buffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}


bool Dx11ShaderConstantBufferContent::updateContent(DirectX11RenderContext *context) {
	if (
			buffer
		&&	last_version != getShaderConstantBuffer()->getChangeVersion()
	) {
		last_version = getShaderConstantBuffer()->getChangeVersion();

		ID3D11DeviceContext *d3d_context = context->getD3DDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result;

		// lock the constant buffer so it can be written to.
		result = d3d_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) {
			return false;
		}

		// copy the matrix buffer content into the buffer
		memcpy(
				mappedResource.pData,
				getShaderConstantBuffer()->getDataPtr(),
				getShaderConstantBuffer()->getTemplate()->getSize()
		);

		// unlock the constant buffer.
		d3d_context->Unmap(buffer, 0);

		return true;
	}

	return false;
}


void Dx11ShaderConstantBufferContent::releaseConstantBuffer() {
	if (buffer) {
		buffer->Release();
		buffer = NULL;
	}

	return;
}
