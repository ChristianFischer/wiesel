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


Dx11ShaderContent::Dx11ShaderContent(Shader *shader) : ShaderContent(shader) {
	this->vertex_shader			= NULL;
	this->pixel_shader			= NULL;
	this->matrix_buffer			= NULL;
	this->matrix_buffer_dirty	= true;

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
	else {
		Log::info
			<< "successfully compiled shader:" << std::endl
			<< std::string(src_buffer->getDataAsCharPtr(), src_buffer->getSize()) 
			<< std::endl << std::endl
		;
	}

	return shader_buffer;
}


Dx11ShaderContent *Dx11ShaderContent::createContentFor(DirectX11RenderContext *context, Shader* shader) {
	if (shader->getSources()->size() > 0) {
		Dx11ShaderContent *dx_shader = new Dx11ShaderContent(shader);
		
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
				dx_shader->shader_buffers[Shader::HLSL_VERTEX_SHADER] = buffer;

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
				dx_shader->shader_buffers[Shader::HLSL_FRAGMENT_SHADER] = buffer;

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
	for(ShaderBufferMap::iterator it=shader_buffers.begin(); it!=shader_buffers.end(); it++) {
		it->second->Release();
	}
	shader_buffers.clear();

	for(PolygonLayoutMap::iterator it=polygon_layouts.begin(); it!=polygon_layouts.end(); it++) {
		it->second->Release();
	}
	polygon_layouts.clear();

	if (matrix_buffer) {
		matrix_buffer->Release();
		matrix_buffer = NULL;
	}

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
	// prepare the matrix búffer
	this->matrix_buffer_content.resize(2);
	this->matrix_buffer_size = matrix_buffer_content.size() * sizeof(matrix4x4);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.ByteWidth				= matrix_buffer_size;
	matrixBufferDesc.Usage					= D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags				= 0;
	matrixBufferDesc.StructureByteStride	= 0;

	// create the constant buffer
	HRESULT result = context->getD3DDevice()->CreateBuffer(&matrixBufferDesc, NULL, &matrix_buffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}


bool Dx11ShaderContent::setProjectionMatrix(const matrix4x4& matrix) {
	memcpy(&(matrix_buffer_content[1]), &matrix, sizeof(matrix));
	matrix_buffer_dirty = true;
	return true;
}


bool Dx11ShaderContent::setModelviewMatrix(const matrix4x4& matrix) {
	memcpy(&(matrix_buffer_content[0]), &matrix, sizeof(matrix));
	matrix_buffer_dirty = true;
	return true;
}


bool Dx11ShaderContent::setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, void *pValue) {
	return false;
}


bool Dx11ShaderContent::uploadAllBuffers(DirectX11RenderContext *render_context) {
	if (matrix_buffer_dirty) {
		ID3D11DeviceContext *d3d_context = render_context->getD3DDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result;

		// lock the constant buffer so it can be written to.
		result = d3d_context->Map(matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) {
			return false;
		}

		// copy the matrix buffer content into the buffer
		memcpy(
				mappedResource.pData,
				matrix_buffer_content.data(),
				matrix_buffer_size
		);

		// unlock the constant buffer.
		d3d_context->Unmap(matrix_buffer, 0);

		// Set the position of the constant buffer in the vertex shader.
		unsigned int buffer_index = 0;

		// Finanly set the constant buffer in the vertex shader with the updated values.
		d3d_context->VSSetConstantBuffers(buffer_index, 1, &matrix_buffer);

		// done - don't need to update the buffer again, until the data has changed
		matrix_buffer_dirty = false;
	}

	return true;
}


bool Dx11ShaderContent::bind(DirectX11RenderContext *render_context) {
	matrix_buffer_dirty = true;
	uploadAllBuffers(render_context);

	ID3D11DeviceContext *d3d_context = render_context->getD3DDeviceContext();

	// Set the vertex and pixel shaders that will be used to render this triangle.
	d3d_context->VSSetShader(vertex_shader, NULL, 0);
	d3d_context->PSSetShader(pixel_shader,  NULL, 0);

	return true;
}


static bool configureInputElement(D3D11_INPUT_ELEMENT_DESC *layout, size_t *size, const VertexBuffer::component &comp) {
	switch(comp.fields) {
		case 1: {
			layout->Format = DXGI_FORMAT_R32_FLOAT;
			(*size) = 4;
			break;
		}

		case 2: {
			layout->Format = DXGI_FORMAT_R32G32_FLOAT;
			(*size) = 8;
			break;
		}

		case 3: {
			layout->Format = DXGI_FORMAT_R32G32B32_FLOAT;
			(*size) = 12;
			break;
		}

		case 4: {
			layout->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			(*size) = 16;
			break;
		}

		default: {
			return false;
		}
	}

	return true;
}


bool Dx11ShaderContent::bind(DirectX11RenderContext *render_context, const VertexBuffer *vertex_buffer) {
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
								shader_buffers[Shader::HLSL_VERTEX_SHADER]->GetBufferPointer(), 
								shader_buffers[Shader::HLSL_VERTEX_SHADER]->GetBufferSize(),
								&layout
		);

		// store the new polygon layout, when successful
		if (SUCCEEDED(result)) {
			polygon_layouts[vertex_buffer_key] = layout;
			layout_it = polygon_layouts.find(vertex_buffer_key);
		}
	}

	if (layout_it != polygon_layouts.end()) {
		render_context->getD3DDeviceContext()->IASetInputLayout(layout_it->second);
		return true;
	}

	return false;
}
