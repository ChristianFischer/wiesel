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
#include "dx11_shader_content.h"
#include "dx11_render_context.h"
#include "dx11_vertexbuffer_content.h"

#include <wiesel/util/log.h>

#include <d3dcompiler.h>
#include <d3dtypes.h>

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::dx11::video;


#define TYPE_VERTEX_SHADER		"vs"
#define TYPE_PIXEL_SHADER		"ps"


Dx11ShaderContent::Dx11ShaderContent(DirectX11RenderContext *context, Shader *shader) : ShaderContent(shader) {
	this->render_context		= context;
	this->vertex_shader			= NULL;
	this->pixel_shader			= NULL;

	return;
}


Dx11ShaderContent::~Dx11ShaderContent() {
	releaseShader();
	return;
}


static ID3D10Blob *compile(DirectX11RenderContext *context, const char *entry_point, const char *type, DataSource *source) {
	DataBuffer* src_buffer = source->getDataBuffer();
	ID3D10Blob* shader_buffer;
	ID3D10Blob* error_buffer;
	HRESULT result;


	// get the shaders api version
	std::string api = context->getScreen()->getVideoDeviceDriver()->getVideoInfo()->shaders.api_version;

	// replace dots with underscores
	for(std::string::iterator it=api.begin(); it!=api.end(); it++) {
		if (*it == '.') {
			*it =  '_';
		}
	}

	std::string shader_type = std::string(type) + "_" + api;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
	#endif

	const D3D_SHADER_MACRO defines[] = {
			// NAME		VALUE
			NULL,		NULL
	};

	// try compiling the shader
	result = D3DCompile(
						src_buffer->getDataAsCharPtr(),
						src_buffer->getSize(),
						"SourceName",
						defines,
						D3D_COMPILE_STANDARD_FILE_INCLUDE,
						entry_point,
						shader_type.c_str(),
						flags,
						0,
						&shader_buffer,
						&error_buffer
	);

	if (FAILED(result)) {
		logmsg(LogLevel_Error, WIESEL_DIRECT3D_LOG_TAG,
				"Could not compile shader:\n%s",
				std::string(src_buffer->getDataAsCharPtr(), src_buffer->getSize()).c_str()
		);

		if (error_buffer) {
			logmsg(LogLevel_Error, WIESEL_DIRECT3D_LOG_TAG,
					std::string(
								reinterpret_cast<const char*>(error_buffer->GetBufferPointer()),
								error_buffer->GetBufferSize()
					).c_str()
			);

			error_buffer->Release();
		}

		return NULL;
	}

	return shader_buffer;
}


Dx11ShaderContent *Dx11ShaderContent::createContentFor(DirectX11RenderContext *context, Shader* shader) {
	if (shader->getSources()->size() > 0) {
		Dx11ShaderContent *dx_shader = new Dx11ShaderContent(context, shader);
		
		if (dx_shader->initializeShader(context) == false) {
			delete dx_shader;
			return NULL;
		}

		DataSource *source;
		bool success = true;

		source = shader->getSource(Shader::HLSL_VERTEX_SHADER);
		if (source) {
			ID3D10Blob *buffer = compile(context, "VertexShaderMain", TYPE_VERTEX_SHADER, source);
			assert(buffer);

			if (buffer) {
				dx_shader->shader_opcode_buffers[Shader::HLSL_VERTEX_SHADER] = buffer;

				HRESULT result = context->getD3DDevice()->CreateVertexShader(
										buffer->GetBufferPointer(),
										buffer->GetBufferSize(),
										NULL,
										&dx_shader->vertex_shader
				);

				if (FAILED(result)) {
					success = false;
				}
			}
			else {
				success = false;
			}
		}

		source = shader->getSource(Shader::HLSL_FRAGMENT_SHADER);
		if (source) {
			ID3D10Blob *buffer = compile(context, "PixelShaderMain", TYPE_PIXEL_SHADER, source);
			assert(buffer);

			if (buffer) {
				dx_shader->shader_opcode_buffers[Shader::HLSL_FRAGMENT_SHADER] = buffer;

				HRESULT result = context->getD3DDevice()->CreatePixelShader(
										buffer->GetBufferPointer(),
										buffer->GetBufferSize(),
										NULL,
										&dx_shader->pixel_shader
				);

				if (FAILED(result)) {
					success = false;
				}
			}
			else {
				success = false;
			}
		}

		// at least one buffer was found?
		if (dx_shader->shader_opcode_buffers.empty()) {
			success = false;
		}

		// try to get handles to all attributes
		if (!dx_shader->bindAttributes(context)) {
			success = false;
		}

		// when failed, delete the shader
		if (!success) {
			dx_shader->releaseShader();
			delete dx_shader;
			return NULL;
		}

		return dx_shader;
	}

	return NULL;
}


bool Dx11ShaderContent::initializeShader(DirectX11RenderContext *context) {
	return true;
}


void Dx11ShaderContent::releaseShader() {
	// clear all opcode buffers
	for(ShaderOpcodeBufferMap::iterator it=shader_opcode_buffers.begin(); it!=shader_opcode_buffers.end(); it++) {
		it->second->Release();
	}
	shader_opcode_buffers.clear();

	for(ShaderConstantBufferEntryMap::iterator it=shader_constant_buffer_entries.begin(); it!=shader_constant_buffer_entries.end(); it++) {
	}
	shader_constant_buffer_entries.clear();

	for(PolygonLayoutMap::iterator it=polygon_layouts.begin(); it!=polygon_layouts.end(); it++) {
		it->second->Release();
	}
	polygon_layouts.clear();

	// release the pixel shader.
	if (pixel_shader) {
		pixel_shader->Release();
		pixel_shader = NULL;
	}

	// release the vertex shader.
	if (vertex_shader) {
		vertex_shader->Release();
		vertex_shader = NULL;
	}

	return;
}


bool Dx11ShaderContent::bindAttributes(DirectX11RenderContext *context) {
	const Shader::ConstantBufferTplList *buffer_templates = getShader()->getConstantBufferTemplates();
	uint32_t vs_slot = 0;
	uint32_t ps_slot = 0;

	for(Shader::ConstantBufferTplList::const_iterator it=buffer_templates->begin(); it!=buffer_templates->end(); it++) {
		ShaderConstantBufferEntry entry;
		entry.vs_slot	= 0xffffffff;
		entry.ps_slot	= 0xffffffff;

		if (it->context & Shader::Context_VertexShader) {
			entry.vs_slot = vs_slot;
			++vs_slot;
		}

		if (it->context & Shader::Context_FragmentShader) {
			entry.ps_slot = ps_slot;
			++ps_slot;
		}

		shader_constant_buffer_entries[it->buffer_template] = entry;
	}

	return true;
}


bool Dx11ShaderContent::assignShaderConstantBuffer(const std::string &name, ShaderConstantBufferContent *buffer_content) {
	Dx11ShaderConstantBufferContent *dx11_buffer_content = dynamic_cast<Dx11ShaderConstantBufferContent*>(buffer_content);

	if (dx11_buffer_content) {
		return assignShaderConstantBuffer(name, dx11_buffer_content);
	}

	return false;
}


bool Dx11ShaderContent::assignShaderConstantBuffer(const std::string &name, Dx11ShaderConstantBufferContent *buffer_content) {
	const ShaderConstantBufferTemplate *buffer_template = getShader()->findConstantBufferTemplate(name);
	if (buffer_template) {
		ShaderConstantBuffer::version_t new_version = buffer_content->getShaderConstantBuffer()->getChangeVersion();
		ShaderConstantBufferEntryMap::iterator entry = shader_constant_buffer_entries.find(buffer_template);

		// create a new entry, if not available
		if (entry != shader_constant_buffer_entries.end()) {

			// update the buffer's content
			buffer_content->updateContent(render_context);
			ID3D11Buffer* dx_buffer = buffer_content->getDx11Buffer();

			if (entry->second.vs_slot != 0xffffffff) {
				render_context->getD3DDeviceContext()->VSSetConstantBuffers(entry->second.vs_slot, 1, &dx_buffer);
			}

			if (entry->second.ps_slot != 0xffffffff) {
				render_context->getD3DDeviceContext()->PSSetConstantBuffers(entry->second.ps_slot, 1, &dx_buffer);
			}
		}

		return true;
	}

	return false;
}


bool Dx11ShaderContent::bind(DirectX11RenderContext *render_context) {
	ID3D11DeviceContext *d3d_context = render_context->getD3DDeviceContext();

	// Set the vertex and pixel shaders that will be used to render this triangle.
	d3d_context->VSSetShader(vertex_shader, NULL, 0);
	d3d_context->PSSetShader(pixel_shader,  NULL, 0);

	return true;
}


static bool configureInputElement(D3D11_INPUT_ELEMENT_DESC *layout, size_t *size, const VertexBuffer::component &comp) {
	(*size) = comp.size;

	switch(comp.fields) {
		case 1: {
			layout->Format = DXGI_FORMAT_R32_FLOAT;
			break;
		}

		case 2: {
			layout->Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		}

		case 3: {
			layout->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}

		case 4: {
			layout->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}

		default: {
			return false;
		}
	}

	return true;
}


bool Dx11ShaderContent::bind(DirectX11RenderContext *render_context, const VertexBuffer *vertex_buffer, const std::vector<Dx11TextureContent*> &textures) {
	std::string vertex_buffer_key = vertex_buffer->getDefaultShaderName();
	PolygonLayoutMap::iterator layout_it = polygon_layouts.find(vertex_buffer_key);

	if (layout_it == polygon_layouts.end()) {
		std::vector<D3D11_INPUT_ELEMENT_DESC> polygon_layout;
		size_t current_offset = 0;

		const Shader::AttributeList *attributes = getShader()->getAttributes();
		for(unsigned int attrib=0; attrib<attributes->size(); attrib++) {
			const Shader::AttributeNamesByIndex *attribute_names = &(attributes->at(attrib));

			for(unsigned int index=0; index<attribute_names->size(); index++) {
				D3D11_INPUT_ELEMENT_DESC layout;
				bool valid_element = true;
				size_t current_size = 0;

				switch(static_cast<Shader::Attribute>(attrib)) {
					case Shader::VertexPosition: {
						layout.SemanticName		= "POSITION";

						if (vertex_buffer->hasPositions() == false || index > 0) {
							valid_element = false;
						}

						valid_element &= configureInputElement(
													&layout,
													&current_size,
													vertex_buffer->getPositionDescription()
						);

						break;
					}

					case Shader::VertexNormal: {
						layout.SemanticName		= "NORMAL";

						if (vertex_buffer->hasNormals() == false || index > 0) {
							valid_element = false;
						}

						valid_element &= configureInputElement(
													&layout,
													&current_size,
													vertex_buffer->getNormalDescription()
						);

						break;
					}

					case Shader::VertexColor: {
						layout.SemanticName		= "COLOR";

						if (vertex_buffer->hasColors() == false || index > 0) {
							valid_element = false;
						}

						valid_element &= configureInputElement(
													&layout,
													&current_size,
													vertex_buffer->getColorDescription()
						);

						break;
					}

					case Shader::VertexTextureCoordinate: {
						layout.SemanticName		= "TEXCOORD";

						if (vertex_buffer->getNumberOfTextureLayers() <= index) {
							valid_element = false;
						}

						valid_element &= configureInputElement(
													&layout,
													&current_size,
													vertex_buffer->getTextureDescription(index)
						);

						break;
					}

					default: {
						valid_element = false;
						break;
					}
				}

				if (!valid_element) {
					continue;
				}

				layout.SemanticIndex		= index;
				layout.InputSlot			= 0;
				layout.AlignedByteOffset	= current_offset;
				layout.InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
				layout.InstanceDataStepRate	= 0;

				// compute the offset for the next component
				current_offset += current_size;

				polygon_layout.push_back(layout);
			}
		}

		ID3D11InputLayout *layout = NULL;
		HRESULT result = render_context->getD3DDevice()->CreateInputLayout(
								polygon_layout.data(),
								polygon_layout.size(),
								shader_opcode_buffers[Shader::HLSL_VERTEX_SHADER]->GetBufferPointer(), 
								shader_opcode_buffers[Shader::HLSL_VERTEX_SHADER]->GetBufferSize(),
								&layout
		);

		// store the new polygon layout, when successful
		if (SUCCEEDED(result)) {
			polygon_layouts[vertex_buffer_key] = layout;
			layout_it = polygon_layouts.find(vertex_buffer_key);
		}
	}

	if (layout_it == polygon_layouts.end()) {
		return false;
	}
	
	// set the according input layout
	render_context->getD3DDeviceContext()->IASetInputLayout(layout_it->second);

	// configure all texture layers
	for(unsigned int i=0; i<vertex_buffer->getNumberOfTextureLayers(); i++) {
		assert(i < textures.size());

		if (i < textures.size() && (textures.at(i) != NULL)) {
			ID3D11ShaderResourceView* resource_view = textures.at(i)->getDx11ShaderResourceView();
			ID3D11SamplerState*       sampler_state = textures.at(i)->getDx11TextureSamplerState();
			render_context->getD3DDeviceContext()->PSSetShaderResources(i, 1, &resource_view);
			render_context->getD3DDeviceContext()->PSSetSamplers(i, 1, &sampler_state);
		}
		else {
			render_context->getD3DDeviceContext()->PSSetShaderResources(i, 1, NULL);
			render_context->getD3DDeviceContext()->PSSetSamplers(i, 1, NULL);
		}
	}

	return true;
}
