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
#include "dx11_indexbuffer_content.h"
#include "dx11_render_context.h"

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


Dx11IndexBufferContent::Dx11IndexBufferContent(IndexBuffer *ib) : IndexBufferContent(ib) {
	this->buffer = NULL;
	return;
}

Dx11IndexBufferContent::~Dx11IndexBufferContent() {
	releaseIndexBuffer();
}



Dx11IndexBufferContent *Dx11IndexBufferContent::createContentFor(DirectX11RenderContext *context, IndexBuffer* vb) {
	Dx11IndexBufferContent *dx11_ib = new Dx11IndexBufferContent(vb);
	if (dx11_ib->initializeIndexBuffer(context) == false) {
		dx11_ib->releaseIndexBuffer();
		delete dx11_ib;
		return NULL;
	}

	return dx11_ib;
}


bool Dx11IndexBufferContent::initializeIndexBuffer(DirectX11RenderContext *context) {
	if (buffer == NULL) {
		D3D11_BUFFER_DESC		buffer_desc;
		D3D11_SUBRESOURCE_DATA	buffer_data;
		HRESULT result;

		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth				= getIndexBuffer()->getBytesPerElement() * getIndexBuffer()->getSize();
		buffer_desc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags			= 0;
		buffer_desc.MiscFlags				= 0;
		buffer_desc.StructureByteStride		= 0;

		buffer_data.pSysMem					= getIndexBuffer()->getDataPtr();
		buffer_data.SysMemPitch				= 0;
		buffer_data.SysMemSlicePitch		= 0;

		result = context->getD3DDevice()->CreateBuffer(&buffer_desc, &buffer_data, &buffer);
		if (FAILED(result)) {
			return false;
		}
	}

	return true;
}


void Dx11IndexBufferContent::releaseIndexBuffer() {
	if (buffer) {
		buffer->Release();
		buffer = NULL;
	}

	return;
}

