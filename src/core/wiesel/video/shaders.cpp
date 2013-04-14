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
#include "shaders.h"
#include "wiesel/util/log.h"
#include <sstream>
#include <string>


using namespace wiesel;
using namespace wiesel::video;
using namespace std;



const char *Shaders::ATTRIBUTE_VERTEX_POSITION					= "vPosition";
const char *Shaders::ATTRIBUTE_VERTEX_NORMAL					= "vNormal";
const char *Shaders::ATTRIBUTE_VERTEX_COLOR						= "vColor";
const char *Shaders::ATTRIBUTE_VERTEX_TEXTURE_COORDINATE		= "vTexCoord";

const char *Shaders::VARYING_COLOR								= "my_color";
const char *Shaders::VARYING_NORMAL								= "my_normal";
const char *Shaders::VARYING_TEXTURE_COORDINATE					= "my_texcoord";

const char *Shaders::UNIFORM_TEXTURE							= "texture";
const char *Shaders::UNIFORM_TEXTURE_SAMPLER					= "sampler";
const char *Shaders::UNIFORM_PROJECTION_MATRIX					= "mProjection";
const char *Shaders::UNIFORM_MODELVIEW_MATRIX					= "mModelview";

const char *Shaders::CONSTANTBUFFER_PROJECTION_MATRIX			= "cbProjection";
const char *Shaders::CONSTANTBUFFER_MODELVIEW_MATRIX			= "cbModelview";



Shaders::Shaders() {
}

Shaders::~Shaders() {
	// release all cached programs
	getShaderCache()->releaseAllObjects();

	// release constant buffer templates
	getShaderConstantBufferTemplateCache()->releaseAllObjects();

	// release all cached shaders
	getGlslVertexShaderCache()->releaseAllObjects();
	getGlslFragmentShaderCache()->releaseAllObjects();
	getHlslVertexShaderCache()->releaseAllObjects();
	getHlslFragmentShaderCache()->releaseAllObjects();

	return;
}


Shaders *Shaders::instance() {
	static Shaders singleton;
	return &singleton;
}


Shader *Shaders::getShaderFor(VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	Shader *shader = getShaderCache()->get(key);
	if (shader == NULL) {
		shader = new Shader();
		DataSource *src_vert_glsl = getGlslVertexShaderSourceFor(shader, vbo);
		DataSource *src_frag_glsl = getGlslFragmentShaderSourceFor(shader, vbo);
		DataSource *src_vert_hlsl = getHlslVertexShaderSourceFor(shader, vbo);
		DataSource *src_pixl_hlsl = getHlslFragmentShaderSourceFor(shader, vbo);

		if (src_vert_glsl) {
			shader->setSource(Shader::GLSL_VERTEX_SHADER,   src_vert_glsl);
		}

		if (src_frag_glsl) {
			shader->setSource(Shader::GLSL_FRAGMENT_SHADER, src_frag_glsl);
		}

		if (src_vert_hlsl) {
			shader->setSource(Shader::HLSL_VERTEX_SHADER,   src_vert_hlsl);
		}

		if (src_pixl_hlsl) {
			shader->setSource(Shader::HLSL_FRAGMENT_SHADER, src_pixl_hlsl);
		}

		getShaderCache()->add(key, shader);
	}

	return shader;
}


ShaderConstantBufferTemplate *Shaders::getProjectionMatrixBufferTemplate() {
	ShaderConstantBufferTemplate *projection_template = NULL;

	// check, if there's already a template in the cache
	projection_template = getShaderConstantBufferTemplateCache()->get(CONSTANTBUFFER_PROJECTION_MATRIX);
	if (projection_template == NULL) {
		projection_template = new ShaderConstantBufferTemplate();
		projection_template->addEntry(TypeMatrix4x4f, 1, UNIFORM_PROJECTION_MATRIX);

		getShaderConstantBufferTemplateCache()->add(CONSTANTBUFFER_PROJECTION_MATRIX, projection_template);
	}

	return projection_template;
}


ShaderConstantBufferTemplate *Shaders::getModelviewMatrixBufferTemplate() {
	ShaderConstantBufferTemplate *modelview_template = NULL;

	// check, if there's already a template in the cache
	modelview_template = getShaderConstantBufferTemplateCache()->get(CONSTANTBUFFER_MODELVIEW_MATRIX);
	if (modelview_template == NULL) {
		modelview_template = new ShaderConstantBufferTemplate();
		modelview_template->addEntry(TypeMatrix4x4f, 1, UNIFORM_MODELVIEW_MATRIX);

		getShaderConstantBufferTemplateCache()->add(CONSTANTBUFFER_MODELVIEW_MATRIX, modelview_template);
	}

	return modelview_template;
}


DataSource *Shaders::getGlslVertexShaderSourceFor(Shader *shader, VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	DataSource *data_source = getGlslVertexShaderCache()->get(key);
	if (data_source == NULL) {
		stringstream ss;

		// modelview & projection matrix
		ss << "uniform mat4 " << UNIFORM_PROJECTION_MATRIX << ';' << endl;
		ss << "uniform mat4 " << UNIFORM_MODELVIEW_MATRIX << ';' << endl;

		// vertex position attribute
		ss << "attribute vec4 " << ATTRIBUTE_VERTEX_POSITION << ';' << endl;

		// color attribute and varying
		if (vbo->hasColors()) {
			ss << "attribute vec4 " << ATTRIBUTE_VERTEX_COLOR << ';' << endl;
			ss << "varying   vec4 " << VARYING_COLOR << ';' << endl;
		}

		// texture coordinate and varying
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			ss << "attribute vec2 " << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE << i << ';' << endl;
			ss << "varying   vec2 " << VARYING_TEXTURE_COORDINATE << i << ';' << endl;
			ss << "uniform   sampler2D " << UNIFORM_TEXTURE << i << ';' << endl;
		}

		// start the main func
		ss << "void main() {" << endl;
		ss << "    gl_Position = " << ATTRIBUTE_VERTEX_POSITION;
		ss << " * " << UNIFORM_MODELVIEW_MATRIX;
		ss << " * " << UNIFORM_PROJECTION_MATRIX;
		ss << ';' << endl;

		// color value will be assigned to the color varying
		if (vbo->hasColors()) {
			ss << "    " << VARYING_COLOR << " = " << ATTRIBUTE_VERTEX_COLOR << ';' << endl;
		}

		// also assign each texture coordinate to it's varying
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			ss << "    " << VARYING_TEXTURE_COORDINATE << i;
			ss << " = "  << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE << i << ';' << endl;
		}

		// end the main func
		ss << "}" << endl;

		// create the data source
		DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(ss.str());
		data_source = new BufferDataSource(buffer);

		// cache this object
		getGlslVertexShaderCache()->add(key, data_source);
	}

	// when given a shader, configure all data members
	if (shader) {
		shader->setAttributeName(Shader::VertexPosition,   0, ATTRIBUTE_VERTEX_POSITION);

		if (vbo->hasNormals()) {
			shader->setAttributeName(Shader::VertexNormal, 0, ATTRIBUTE_VERTEX_NORMAL);
		}

		if (vbo->hasColors()) {
			shader->setAttributeName(Shader::VertexColor,  0, ATTRIBUTE_VERTEX_COLOR);
		}

		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE;
			tex_attr << i;
			shader->setAttributeName(Shader::VertexTextureCoordinate, i, tex_attr.str());
		}

		shader->addConstantBuffer(
								CONSTANTBUFFER_MODELVIEW_MATRIX,
								Shader::Context_VertexShader,
								getModelviewMatrixBufferTemplate()
		);

		shader->addConstantBuffer(
								CONSTANTBUFFER_PROJECTION_MATRIX,
								Shader::Context_VertexShader,
								getProjectionMatrixBufferTemplate()
		);
	}

	return data_source;
}


DataSource *Shaders::getGlslFragmentShaderSourceFor(Shader *shader, VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	DataSource *data_source = getGlslFragmentShaderCache()->get(key);
	if (data_source == NULL) {
		stringstream ss;
		int color_sources = 0;

		// set the shader precision
		ss << "#ifdef GL_ES" << endl;
		ss << "precision mediump float;" << endl;
		ss << "#endif" << endl << endl;

		// color varying
		if (vbo->hasColors()) {
			ss << "varying vec4 " << VARYING_COLOR << ';' << endl;
			++color_sources;
		}

		// texture varying and uniform
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			ss << "varying vec2 " << VARYING_TEXTURE_COORDINATE << i << ';' << endl;
			ss << "uniform sampler2D " << UNIFORM_TEXTURE << i << ';' << endl;
			++color_sources;
		}

		// start the main func
		ss << "void main() {" << endl;

		switch(color_sources) {
			// when no color source is available, set the color of the fragment to a pretty pink
			case 0: {
				ss << "    gl_FragColor = vec4(1, 0, 1, 1);" << endl;
				break;
			}

			// when just one color source is available, assign it directly to gl_FragColor
			case 1: {
				ss << "    gl_FragColor = ";

				if (vbo->hasColors()) {
					ss << VARYING_COLOR;
				}

				if (vbo->hasTextures()) {
					ss << "texture2D(";
					ss << UNIFORM_TEXTURE << '0' << ", ";
					ss << VARYING_TEXTURE_COORDINATE << '0';
					ss << ')';
				}

				ss << ';' << endl;

				break;
			}

			// multiple color sources will be multiplied
			default: {
				ss << "    vec4 color = vec4(1, 1, 1, 1);" << endl;

				// apply the vertex color
				if (vbo->hasColors()) {
					ss << "    color *= " << VARYING_COLOR << ';' << endl;
				}

				// apply all texture colors
				for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
					ss << "    color *= texture2D(";
					ss << UNIFORM_TEXTURE << i << ", ";
					ss << VARYING_TEXTURE_COORDINATE << i;
					ss << ");" << endl;
				}

				// assign the multiplied value to gl_FragColor
				ss << "    gl_FragColor = color;" << endl;

				break;
			}
		}

		// end the main func
		ss << "}" << endl;

		// create the data source
		DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(ss.str());
		data_source = new BufferDataSource(buffer);

		// cache this object
		getGlslFragmentShaderCache()->add(key, data_source);
	}

	if (shader) {
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << UNIFORM_TEXTURE;
			tex_attr << i;
			shader->setAttributeName(Shader::Texture, i, tex_attr.str());
		}
	}

	return data_source;
}




DataSource *Shaders::getHlslVertexShaderSourceFor(Shader *shader, VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	DataSource *data_source = getHlslVertexShaderCache()->get(key);
	if (data_source == NULL) {
		stringstream ss;

		// modelview matrix buffer
		ss << "cbuffer " << CONSTANTBUFFER_MODELVIEW_MATRIX << " {" << endl;
		ss << "  matrix " << UNIFORM_MODELVIEW_MATRIX << ';' << endl;
		ss << "};" << endl << endl;

		// projection matrix buffer
		ss << "cbuffer " << CONSTANTBUFFER_PROJECTION_MATRIX << " {" << endl;
		ss << "  matrix " << UNIFORM_PROJECTION_MATRIX << ';' << endl;
		ss << "};" << endl << endl;

		// vertex shader input
		{
			ss << "struct VertexInputStruct {" << endl;

			// vertex position attribute
			ss << "    float4 " << ATTRIBUTE_VERTEX_POSITION << " : POSITION;" << endl;

			// color attribute
			if (vbo->hasColors()) {
				ss << "    float4 " << ATTRIBUTE_VERTEX_COLOR << " : COLOR;" << endl;
			}

			// texture coordinate and varying
			for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
				ss << "    float2 " << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE << i << " : TEXCOORD" << i << ';' << endl;
			}

			ss << "};" << endl << endl;
		}

		// pixel shader input
		{
			ss << "struct PixelInputStruct {" << endl;

			// vertex position attribute
			ss << "    float4 " << ATTRIBUTE_VERTEX_POSITION << " : SV_POSITION;" << endl;

			// color attribute
			if (vbo->hasColors()) {
				ss << "    float4 " << VARYING_COLOR << " : COLOR;" << endl;
			}

			// texture coordinate and varying
			for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
				ss << "    float2 " << VARYING_TEXTURE_COORDINATE << i << " : TEXCOORD" << i << ';' << endl;
			}

			ss << "};" << endl << endl;
		}

		// start the main func
		ss << "PixelInputStruct VertexShaderMain(VertexInputStruct input) {" << endl;
		ss << "    input." << ATTRIBUTE_VERTEX_POSITION << ".w = 1.0f;" << endl;
		ss << "    PixelInputStruct output;" << endl;
		ss << "    output." << ATTRIBUTE_VERTEX_POSITION << " = mul(input." << ATTRIBUTE_VERTEX_POSITION << ", " << UNIFORM_MODELVIEW_MATRIX << ");" << endl;
		ss << "    output." << ATTRIBUTE_VERTEX_POSITION << " = mul(output." << ATTRIBUTE_VERTEX_POSITION << ", " << UNIFORM_PROJECTION_MATRIX << ");" << endl;

		// color value will be assigned to the color varying
		if (vbo->hasColors()) {
			ss << "    output." << VARYING_COLOR << " = input." << ATTRIBUTE_VERTEX_COLOR << ';' << endl;
		}

		// also assign each texture coordinate to it's varying
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			ss << "    output." << VARYING_TEXTURE_COORDINATE << i;
			ss << " = input." << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE << i << ';' << endl;
		}

		// end the main func
		ss << "    return output;" << endl;
		ss << "}" << endl;

		// create the data source
		DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(ss.str());
		data_source = new BufferDataSource(buffer);

		// cache this object
		getHlslVertexShaderCache()->add(key, data_source);
	}

	// when given a shader, configure all data members
	if (shader) {
		shader->setAttributeName(Shader::VertexPosition,   0, ATTRIBUTE_VERTEX_POSITION);

		if (vbo->hasNormals()) {
			shader->setAttributeName(Shader::VertexNormal, 0, ATTRIBUTE_VERTEX_NORMAL);
		}

		if (vbo->hasColors()) {
			shader->setAttributeName(Shader::VertexColor,  0, ATTRIBUTE_VERTEX_COLOR);
		}

		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE;
			tex_attr << i;
			shader->setAttributeName(Shader::VertexTextureCoordinate, i, tex_attr.str());
		}

		shader->addConstantBuffer(
								CONSTANTBUFFER_MODELVIEW_MATRIX,
								Shader::Context_VertexShader,
								getModelviewMatrixBufferTemplate()
		);

		shader->addConstantBuffer(
								CONSTANTBUFFER_PROJECTION_MATRIX,
								Shader::Context_VertexShader,
								getProjectionMatrixBufferTemplate()
		);
	}

	return data_source;
}


DataSource *Shaders::getHlslFragmentShaderSourceFor(Shader *shader, VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	DataSource *data_source = getHlslFragmentShaderCache()->get(key);
	if (data_source == NULL) {
		stringstream ss;
		int color_sources = 0;

		// texture coordinate and varying
		if (vbo->getNumberOfTextureLayers() > 0) {
			for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
				ss << "Texture2D    " << UNIFORM_TEXTURE << i << ';' << endl;
				ss << "SamplerState " << UNIFORM_TEXTURE_SAMPLER << i << ';' << endl;
			}
			
			ss << std::endl;
		}

		// pixel shader input
		{
			ss << "struct PixelInputStruct {" << endl;

			// vertex position attribute
			ss << "    float4 " << ATTRIBUTE_VERTEX_POSITION << " : SV_POSITION;" << endl;

			// color attribute
			if (vbo->hasColors()) {
				ss << "    float4 " << VARYING_COLOR << " : COLOR;" << endl;
				++color_sources;
			}

			// texture coordinate and varying
			for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
				ss << "    float2 " << VARYING_TEXTURE_COORDINATE << i << " : TEXCOORD" << i << ';' << endl;
				++color_sources;
			}

			ss << "};" << endl << endl;
		}

/*
		// texture varying and uniform
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			ss << "varying vec2 " << VARYING_TEXTURE_COORDINATE << i << ';' << endl;
			ss << "uniform sampler2D " << UNIFORM_TEXTURE << i << ';' << endl;
			++color_sources;
		}
*/

		// start the main func
		ss << "float4 PixelShaderMain(PixelInputStruct input) : SV_TARGET {" << endl;

		switch(color_sources) {
			// when no color source is available, set the color of the fragment to a pretty pink
			case 0: {
				ss << "    return float4(1, 0, 1, 1);" << endl;
				break;
			}

			// when just one color source is available, return it directly
			case 1: {
				ss << "    return ";

				if (vbo->hasColors()) {
					ss << "input." << VARYING_COLOR;
				}

				if (vbo->hasTextures()) {
					ss << UNIFORM_TEXTURE << '0' << ".Sample(";
					ss << UNIFORM_TEXTURE_SAMPLER << '0' << ", input.";
					ss << VARYING_TEXTURE_COORDINATE << '0';
					ss << ')';
				}

				ss << ';' << endl;

				break;
			}

			// multiple color sources will be multiplied
			default: {
				ss << "    float color = float4(1, 1, 1, 1);" << endl;

				// apply the vertex color
				if (vbo->hasColors()) {
					ss << "    color = mul(color, input." << VARYING_COLOR << ");" << endl;
				}

				// apply all texture colors
				for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
					ss << "    color *= ";
					ss << UNIFORM_TEXTURE << i << ".Sample(";
					ss << UNIFORM_TEXTURE_SAMPLER << i << ", input.";
					ss << VARYING_TEXTURE_COORDINATE << i;
					ss << ");" << endl;
				}

				// assign the multiplied value to gl_FragColor
				ss << "    return color;" << endl;

				break;
			}
		}

		// end the main func
		ss << "}" << endl;

		// create the data source
		DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(ss.str());
		data_source = new BufferDataSource(buffer);

		// cache this object
		getHlslFragmentShaderCache()->add(key, data_source);
	}

	if (shader) {
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << UNIFORM_TEXTURE;
			tex_attr << i;
			shader->setAttributeName(Shader::Texture, i, tex_attr.str());
		}
	}

	return data_source;
}
