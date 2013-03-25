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
#include "dx11_render_context.h"
#include "dx11_vertexbuffer_content.h"
#include "dx11_indexbuffer_content.h"

#include <wiesel/util/log.h>
#include <wiesel/video/shader.h>
#include <wiesel/video/texture.h>

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;



DirectX11RenderContext::DirectX11RenderContext(Screen *screen) : RenderContext(screen) {
	driver_type						= D3D_DRIVER_TYPE_NULL;
	feature_level					= D3D_FEATURE_LEVEL_11_0;
	d3d_device						= NULL;
	d3d_device_context				= NULL;
	swap_chain						= NULL;
	render_target_view				= NULL;
	blendstate_enabled				= NULL;

	this->active_shader				= NULL;
	this->active_shader_content		= NULL;

	return;
}


DirectX11RenderContext::~DirectX11RenderContext() {
	releaseContext();
	return;
}



DirectX11RenderContext *DirectX11RenderContext::createContextWithWindowHandle(HWND hWnd, Screen *screen, VideoInfo *pVideoInfo) {
	DirectX11RenderContext *context = new DirectX11RenderContext(screen);
	HRESULT result = S_OK;

	// TODO: move into configuration
	context->vsync			= true;
	context->fullscreen		= false;

	// get the window dimensions
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width  = rc.right  - rc.left;
	UINT height = rc.bottom - rc.top;

	// initialize the swap chain description
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount							= 1;
	sd.BufferDesc.Width						= width;
	sd.BufferDesc.Height					= height;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.RefreshRate.Numerator		= 0;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	sd.OutputWindow							= hWnd;
	sd.SampleDesc.Count						= 1;
	sd.SampleDesc.Quality					= 0;
	sd.Windowed								= context->fullscreen ? FALSE : TRUE;

	IDXGIFactory*	factory			= NULL;
	IDXGIAdapter*	adapter			= NULL;
	IDXGIOutput*	adapterOutput	= NULL;

	// create the factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// create the display adapter
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// get the primary output adapter (monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// get the adapter description
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// try to copy the informations from the adapter to our video info struct
	if (pVideoInfo) {
		pVideoInfo->api			= "DirectX";
		pVideoInfo->shaders.api	= "HLSL";

		unsigned int stringLength;
		char str[1024];

		int error = wcstombs_s(&stringLength, str, 1024, adapterDesc.Description, 1024);
		if (error == 0) {
			pVideoInfo->renderer = str;
		}
	}

	// get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// create a list containing all available display modes
	DXGI_MODE_DESC *displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// when vsync enabled, find a display mode which matches the requested resolution
	if (context->vsync) {
		for(unsigned int i=0; i<numModes; i++) {
			if (
					displayModeList[i].Width  == static_cast<unsigned int>(width)
				&&	displayModeList[i].Height == static_cast<unsigned int>(height)
			) {
				sd.BufferDesc.RefreshRate.Numerator		= displayModeList[i].RefreshRate.Numerator;
				sd.BufferDesc.RefreshRate.Denominator	= displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// cleanup objects, which are no longer requred
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = NULL;

	// set the feature levels we want to use
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// we prefer hardware acceleration, but allow fallback,
	// if no DX11 hardware is available
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes   = ARRAYSIZE(driverTypes);
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// additional flags
	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	for(UINT driverTypeIndex=0; driverTypeIndex<numDriverTypes; driverTypeIndex++) {
		context->driver_type = driverTypes[driverTypeIndex];

		result = D3D11CreateDeviceAndSwapChain(
										NULL,
										context->driver_type,
										NULL,
										createDeviceFlags,
										featureLevels, numFeatureLevels,
										D3D11_SDK_VERSION,
										&sd,
										&context->swap_chain,
										&context->d3d_device,
										&context->feature_level,
										&context->d3d_device_context
		);

		if (SUCCEEDED(result)) {
			break;
		}
	}

	if (FAILED(result)) {
		delete context;
		return NULL;
	}

	// store video information based on the current feature level
	// there's a guaranteed minimal feature set per feature level
	// see: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476876%28v=vs.85%29.aspx
	if (pVideoInfo) {
		switch(context->feature_level) {
		case D3D_FEATURE_LEVEL_11_1:
			pVideoInfo->api_version					= "11.1";
			pVideoInfo->shaders.api_version			= "5.0";
			pVideoInfo->textures.max_size			= 16384;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= false;
			break;

		case D3D_FEATURE_LEVEL_11_0:
			pVideoInfo->api_version					= "11.0";
			pVideoInfo->shaders.api_version			= "5.0";
			pVideoInfo->textures.max_size			= 16384;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= false;
			break;

		case D3D_FEATURE_LEVEL_10_1:
			pVideoInfo->api_version					= "10.1";
			pVideoInfo->shaders.api_version			= "4.x";
			pVideoInfo->textures.max_size			= 8192;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= false;
			break;

		case D3D_FEATURE_LEVEL_10_0:
			pVideoInfo->api_version					= "10.0";
			pVideoInfo->shaders.api_version			= "4.0";
			pVideoInfo->textures.max_size			= 8192;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= false;
			break;

		case D3D_FEATURE_LEVEL_9_3:
			pVideoInfo->api_version					= "9.3";
			pVideoInfo->shaders.api_version			= "2.0";
			pVideoInfo->textures.max_size			= 4096;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= true;
			break;

		case D3D_FEATURE_LEVEL_9_2:
			pVideoInfo->api_version					= "9.2";
			pVideoInfo->shaders.api_version			= "2.0";
			pVideoInfo->textures.max_size			= 2048;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= true;
			break;

		case D3D_FEATURE_LEVEL_9_1:
			pVideoInfo->api_version					= "9.1";
			pVideoInfo->shaders.api_version			= "2.0";
			pVideoInfo->textures.max_size			= 2048;
			pVideoInfo->textures.max_texture_units	= 16;
			pVideoInfo->textures.requires_pot		= true;
			break;

		default:
			pVideoInfo->api_version	= "<unknown version>";
			break;
		}
	}

	// get a pointer to the backbuffer
	ID3D11Texture2D* backbuffer = NULL;
	result = context->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	if (FAILED(result)) {
		delete context;
		return NULL;
	}

	// create a render target view
	result = context->d3d_device->CreateRenderTargetView(backbuffer, NULL, &context->render_target_view);
	backbuffer->Release();
	if (FAILED(result)) {
		delete context;
		return NULL;
	}

	// depth buffer description
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width				= width;
	depthBufferDesc.Height				= height;
	depthBufferDesc.MipLevels			= 1;
	depthBufferDesc.ArraySize			= 1;
	depthBufferDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count	= 1;
	depthBufferDesc.SampleDesc.Quality	= 0;
	depthBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags		= 0;
	depthBufferDesc.MiscFlags			= 0;

	// create a texture which will contain our depth and stencil buffer
	result = context->d3d_device->CreateTexture2D(&depthBufferDesc, NULL, &context->depth_stencil_buffer);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// initialize the stencil buffer description
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable					= true;
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable					= true;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = context->d3d_device->CreateDepthStencilState(&depthStencilDesc, &context->depth_stencil_state);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// assign the depth stencil state to the device context
	context->d3d_device_context->OMSetDepthStencilState(context->depth_stencil_state, 1);

	// initailze the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice		= 0;

	// Create the depth stencil view.
	result = context->d3d_device->CreateDepthStencilView(context->depth_stencil_buffer, &depthStencilViewDesc, &context->depth_stencil_view);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// bind the render target view and depth stencil buffer to the output render pipeline.
	context->d3d_device_context->OMSetRenderTargets(1, &context->render_target_view, context->depth_stencil_view);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable		= false;
	rasterDesc.CullMode						= D3D11_CULL_NONE;
	rasterDesc.DepthBias					= 0;
	rasterDesc.DepthBiasClamp				= 0.0f;
	rasterDesc.DepthClipEnable				= true;
	rasterDesc.FillMode						= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise		= false;
	rasterDesc.MultisampleEnable			= false;
	rasterDesc.ScissorEnable				= false;
	rasterDesc.SlopeScaledDepthBias			= 0.0f;

	// create the rasterizer state
	result = context->d3d_device->CreateRasterizerState(&rasterDesc, &context->raster_state);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// create a blend state description
	D3D11_BLEND_DESC blendstate_enabled_desc;
	ZeroMemory(&blendstate_enabled_desc, sizeof(D3D11_BLEND_DESC));
	blendstate_enabled_desc.RenderTarget[0].BlendEnable				= TRUE;
	blendstate_enabled_desc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendstate_enabled_desc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendstate_enabled_desc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	blendstate_enabled_desc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendstate_enabled_desc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	blendstate_enabled_desc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendstate_enabled_desc.RenderTarget[0].RenderTargetWriteMask	= 0x0f;
 
	result = context->d3d_device->CreateBlendState(&blendstate_enabled_desc, &context->blendstate_enabled);
	if (FAILED(result)) {
		delete context;
		return false;
	}

	// Now set the rasterizer state.
	context->d3d_device_context->RSSetState(context->raster_state);		

	// initialize the viewport
	context->onSizeChanged(dimension(sd.BufferDesc.Width, sd.BufferDesc.Height));

	return context;
}


void DirectX11RenderContext::initContext() {
	return;
}


void DirectX11RenderContext::releaseContext() {
	setShader(NULL);
	clearTextures();

	// switch back to windowed mode
	if (swap_chain) {
		swap_chain->SetFullscreenState(false, NULL);
	}

	if (raster_state) {
		raster_state->Release();
		raster_state = NULL;
	}

	if (blendstate_enabled) {
		blendstate_enabled->Release();
		blendstate_enabled = NULL;
	}

	if (depth_stencil_view) {
		depth_stencil_view->Release();
		depth_stencil_view = NULL;
	}

	if (depth_stencil_state) {
		depth_stencil_state->Release();
		depth_stencil_state = NULL;
	}

	if (depth_stencil_buffer) {
		depth_stencil_buffer->Release();
		depth_stencil_buffer = NULL;
	}

	if (render_target_view) {
		render_target_view->Release();
		render_target_view = NULL;
	}

	if (d3d_device_context) {
		d3d_device_context->ClearState();
		d3d_device_context->Release();
		d3d_device_context = NULL;
	}

	if (swap_chain) {
		swap_chain->Release();
		swap_chain = NULL;
	}

	if (d3d_device) {
		d3d_device->Release();
		d3d_device = NULL;
	}

	return;
}


void DirectX11RenderContext::onSizeChanged(const dimension& size) {
	// setup viewport
	if (d3d_device_context) {
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width    = size.width;
		vp.Height   = size.height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		d3d_device_context->RSSetViewports(1, &vp);
	}

	return;
}


void DirectX11RenderContext::preRender() {
	// Just fill the screen with a color.
	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	d3d_device_context->ClearRenderTargetView(render_target_view, clear_color);
	d3d_device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// enable alpha blending
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	d3d_device_context->OMSetBlendState(blendstate_enabled, blendFactor, 0xffffffff);

	return;
}


void DirectX11RenderContext::postRender() {
	// reset all gl objects
	setShader(NULL);
	clearTextures();

	// display screen
	if (vsync) {
		swap_chain->Present(1, 0);
	}
	else {
		swap_chain->Present(0, 0);
	}

	return;
}



void DirectX11RenderContext::setProjectionMatrix(const matrix4x4& matrix) {
	this->projection = matrix;

	if (active_shader_content) {
		active_shader_content->setProjectionMatrix(this->projection);
	}

	return;
}


void DirectX11RenderContext::setModelviewMatrix(const matrix4x4& matrix) {
	if (active_shader_content) {
		active_shader_content->setModelviewMatrix(matrix);
	}

	return;
}


void DirectX11RenderContext::setShader(Shader* shader) {
	if (this->active_shader != shader) {
		// clear the old shader
		if (this->active_shader) {
			this->active_shader->release();
			this->active_shader = NULL;
		}

		// ... and it's implementation
		if (this->active_shader_content) {
			this->active_shader_content->release();
			this->active_shader_content = NULL;
		}

		// store the new shader
		if (shader) {
			this->active_shader = shader;
			this->active_shader->retain();

			// load the shader implementation on demand
			if(!active_shader->isLoaded()) {
				active_shader->loadContentFrom(getScreen());
			}

			Dx11ShaderContent *dx_shader_content = dynamic_cast<Dx11ShaderContent*>(active_shader->getContent());
			if (dx_shader_content) {
				this->active_shader_content = dx_shader_content;
				this->active_shader_content->retain();
			}
		}

		// bind the new shader, if any
		if (active_shader_content) {
			active_shader_content->bind(this);

			// update projection matrix for the current shader
			active_shader_content->setProjectionMatrix(getProjectionMatrix());
		}
		else {
			d3d_device_context->VSSetShader(NULL, NULL, 0);
			d3d_device_context->PSSetShader(NULL, NULL, 0);
		}
	}

	return;
}


void DirectX11RenderContext::setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, void *pValue) {
	if (active_shader_content) {
		active_shader_content->setShaderValue(name, type, elements, pValue);
	}

	return;
}



void DirectX11RenderContext::setTexture(uint16_t index, Texture* texture) {
	// check if the texture list is big enough.
	// when this assert fails, there may be missing a prepareTextures call
	assert(index < active_textures.size());

	Texture *active_texture = this->active_textures[index];
	if (active_texture != texture) {
		Dx11TextureContent *active_texture_content = this->active_textures_content[index];

		// clear the old texture
		if (active_texture) {
			active_texture->release();
			active_texture = NULL;
		}

		// and it's implementation
		if (active_texture_content) {
			active_texture_content->release();
			active_texture_content = NULL;
		}

		// store the new texture
		if (texture) {
			active_texture = texture;
			active_texture->retain();

			Dx11TextureContent *dx11_texture_content = dynamic_cast<Dx11TextureContent*>(active_texture->getContent());
			if (dx11_texture_content) {
				active_texture_content = dx11_texture_content;
				active_texture_content->retain();
			}
		}

		// write active textures into the texture list
		active_textures[index]         = active_texture;
		active_textures_content[index] = active_texture_content;
	}

	return;
}


void DirectX11RenderContext::prepareTextureLayers(uint16_t layers) {
	if (active_textures.size() < layers) {
		active_textures.resize(layers, NULL);
		active_textures_content.resize(layers, NULL);
	}
	else {
		// clear all textures above the requested layer number
		for(uint16_t l=layers; l<active_textures.size(); l++) {
			setTexture(l, NULL);
		}
	}

	return;
}

void DirectX11RenderContext::clearTextures() {
	prepareTextureLayers(0);
}



void DirectX11RenderContext::draw(Primitive primitive, const VertexBuffer *vertices) {
	if (bind(vertices)) {
		D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

		switch(primitive) {
			case Triangles: {
				topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			}

			case TriangleStrip: {
				topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
				break;
			}

			default: {
				topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
				break;
			}
		}

		if (topology != D3D_PRIMITIVE_TOPOLOGY_UNDEFINED) {
			getD3DDeviceContext()->IASetPrimitiveTopology(topology);
			getD3DDeviceContext()->Draw(vertices->getSize(), 0);
		}

		unbind(vertices);
	}

	return;
}


void DirectX11RenderContext::draw(Primitive primitive, const VertexBuffer *vertices, const IndexBuffer *indices) {
	if (
			bind(vertices)
		&&	bind(indices)
	) {
		D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

		switch(primitive) {
			case Triangles: {
				topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			}

			case TriangleStrip: {
				topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
				break;
			}

			default: {
				topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
				break;
			}
		}

		if (topology != D3D_PRIMITIVE_TOPOLOGY_UNDEFINED) {
			getD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			getD3DDeviceContext()->DrawIndexed(indices->getSize(), 0, 0);
		}

		unbind(indices);
		unbind(vertices);
	}

	return;
}


bool DirectX11RenderContext::bind(const IndexBuffer* index_buffer) {
	if (index_buffer) {
		// create the hardware buffer on demand
		if(!index_buffer->isLoaded()) {
			const_cast<IndexBuffer*>(index_buffer)->loadContentFrom(getScreen());
		}

		const Dx11IndexBufferContent *dx11_index_buffer;
		dx11_index_buffer = dynamic_cast<const Dx11IndexBufferContent*>(index_buffer->getContent());

		if (dx11_index_buffer) {
			DXGI_FORMAT format;

			switch(index_buffer->getBytesPerElement()) {
				case 1: {
					format = DXGI_FORMAT_R8_UINT;
					break;
				}

				case 2: {
					format = DXGI_FORMAT_R16_UINT;
					break;
				}

				case 4: {
					format = DXGI_FORMAT_R32_UINT;
					break;
				}

				default: {
					return false;
				}
			}

			getD3DDeviceContext()->IASetIndexBuffer(
						dx11_index_buffer->getDx11Buffer(),
						format,
						0
			);

			return true;
		}
	}

	return false;
}


void DirectX11RenderContext::unbind(const IndexBuffer *index_buffer) {
	if (index_buffer) {
		getD3DDeviceContext()->IASetIndexBuffer(
					NULL,
					DXGI_FORMAT_UNKNOWN,
					0
		);
	}

	return;
}


bool DirectX11RenderContext::bind(const VertexBuffer* vertex_buffer) {
	if (vertex_buffer && active_shader_content) {
		// try to bind this vertex buffer to the current shader
		if (active_shader_content->bind(this, vertex_buffer, active_textures_content) == false) {
			return false;
		}

		// prepare the shader
		active_shader_content->uploadAllBuffers(this);

		// create the hardware buffer on demand
		if(!vertex_buffer->isLoaded()) {
			const_cast<VertexBuffer*>(vertex_buffer)->loadContentFrom(getScreen());
		}

		const Dx11VertexBufferContent *dx11_vertex_buffer;
		dx11_vertex_buffer = dynamic_cast<const Dx11VertexBufferContent*>(vertex_buffer->getContent());

		if (dx11_vertex_buffer) {
			ID3D11Buffer *buffer = dx11_vertex_buffer->getDx11Buffer();
			unsigned int stride  = vertex_buffer->getVertexSize();
			unsigned int offset  = 0;

			getD3DDeviceContext()->IASetVertexBuffers(
										0,
										1,
										&buffer,
										&stride,
										&offset
			);
		}

		return true;
	}

	return false;
}


void DirectX11RenderContext::unbind(const VertexBuffer* vertex_buffer) {
	if (vertex_buffer) {
		getD3DDeviceContext()->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	}

	return;
}

