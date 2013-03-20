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
#include "dx11_vertexbuffer_content.h"
#include "dx11_render_context.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


Dx11VertexBufferContent::Dx11VertexBufferContent(VertexBuffer *vb) : VertexBufferContent(vb) {
	this->buffer = NULL;
	return;
}

Dx11VertexBufferContent::~Dx11VertexBufferContent() {
	releaseVertexBuffer();
}



Dx11VertexBufferContent *Dx11VertexBufferContent::createContentFor(DirectX11RenderContext *context, VertexBuffer* vb) {
	Dx11VertexBufferContent *dx11_vb = new Dx11VertexBufferContent(vb);
	if (dx11_vb->initializeVertexBuffer(context) == false) {
		dx11_vb->releaseVertexBuffer();
		delete dx11_vb;
		return NULL;
	}

	return dx11_vb;
}


bool Dx11VertexBufferContent::initializeVertexBuffer(DirectX11RenderContext *context) {
	if (buffer == NULL) {
		D3D11_BUFFER_DESC		buffer_desc;
		D3D11_SUBRESOURCE_DATA	buffer_data;
		HRESULT result;

		buffer_desc.Usage					= D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth				= getVertexBuffer()->getVertexSize() * getVertexBuffer()->getSize();
		buffer_desc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags			= 0;
		buffer_desc.MiscFlags				= 0;
		buffer_desc.StructureByteStride		= 0;

		buffer_data.pSysMem					= getVertexBuffer()->getDataPtr();
		buffer_data.SysMemPitch				= 0;
		buffer_data.SysMemSlicePitch		= 0;

		result = context->getD3DDevice()->CreateBuffer(&buffer_desc, &buffer_data, &buffer);
		if (FAILED(result)) {
			return false;
		}
	}

	return true;
}


void Dx11VertexBufferContent::releaseVertexBuffer() {
	if (buffer) {
		buffer->Release();
		buffer = NULL;
	}

	return;
}

