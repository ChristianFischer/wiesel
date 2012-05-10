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
using namespace std;



const char *Shaders::ATTRIBUTE_VERTEX_POSITION					= "vPosition";
const char *Shaders::ATTRIBUTE_VERTEX_NORMAL					= "vNormal";
const char *Shaders::ATTRIBUTE_VERTEX_COLOR						= "vColor";
const char *Shaders::ATTRIBUTE_VERTEX_TEXTURE_COORDINATE		= "vTexCoord";
const char *Shaders::UNIFORM_TEXTURE							= "texture";
const char *Shaders::UNIFORM_MODELVIEW_MATRIX					= "u_mvp";
const char *Shaders::VARYING_COLOR								= "my_color";
const char *Shaders::VARYING_NORMAL								= "my_normal";
const char *Shaders::VARYING_TEXTURE_COORDINATE					= "my_texcoord";



Shaders::Shaders() {
}

Shaders::~Shaders() {
	// release all cached programs
	getShaderProgramCache()->releaseAllObjects();

	// release all cached shaders
	getVertexShaderCache()->releaseAllObjects();
	getFragmentShaderCache()->releaseAllObjects();

	return;
}


Shaders *Shaders::instance() {
	static Shaders singleton;
	return &singleton;
}


ShaderProgram *Shaders::getShaderFor(VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();

	// check, if there's already an shader in the vertex shader cache
	ShaderProgram *prog = getShaderProgramCache()->get(key);
	if (prog) {
		return prog;
	}
	else {
		Shader *vert = getVertexShaderFor(vbo);
		Shader *frag = getFragmentShaderFor(vbo);

		if (vert && frag) {
			prog = new ShaderProgram();
			prog->attach(vert);
			prog->attach(frag);

			if (prog->link()) {
				// cache this object
				getShaderProgramCache()->add(key, prog);

				return prog;
			}
		}
	}

	return NULL;
}


Shader *Shaders::getVertexShaderFor(VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();
	stringstream ss;

	// check, if there's already an shader in the vertex shader cache
	Shader *shader = getVertexShaderCache()->get(key);
	if (shader) {
		return shader;
	}

	// modelview matrix
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
	ss << " * " << UNIFORM_MODELVIEW_MATRIX << ';' << endl;

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

	// compile the shader
	string shader_source = ss.str();
	shader = Shader::compile(shader_source, ShaderType_VertexShader);

	if (shader) {
		shader->uniform_modelview_matrix	= UNIFORM_MODELVIEW_MATRIX;
		shader->attrib_vertex_position		= ATTRIBUTE_VERTEX_POSITION;

		if (vbo->hasNormals()) {
			shader->attrib_vertex_normal	= ATTRIBUTE_VERTEX_NORMAL;
		}

		if (vbo->hasColors()) {
			shader->attrib_vertex_color		= ATTRIBUTE_VERTEX_COLOR;
		}

		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << ATTRIBUTE_VERTEX_TEXTURE_COORDINATE;
			tex_attr << i;
			shader->attrib_vertex_texcoords.push_back(tex_attr.str());
		}

		// cache this object
		getVertexShaderCache()->add(key, shader);
	}

	return shader;
}


Shader *Shaders::getFragmentShaderFor(VertexBuffer* vbo) {
	string key = vbo->getDefaultShaderName();
	stringstream ss;
	int color_sources = 0;

	// check, if there's already an shader in the vertex shader cache
	Shader *shader = getFragmentShaderCache()->get(key);
	if (shader) {
		return shader;
	}

	// set the shader precision
	ss << "precision mediump float;" << endl;

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

	// compile the shader
	string shader_source = ss.str();
	shader = Shader::compile(shader_source, ShaderType_FragmentShader);

	if (shader) {
		for(int i=0; i<vbo->getNumberOfTextureLayers(); i++) {
			stringstream tex_attr;
			tex_attr << UNIFORM_TEXTURE;
			tex_attr << i;
			shader->attrib_vertex_textures.push_back(tex_attr.str());
		}

		// cache this object
		getFragmentShaderCache()->add(key, shader);
	}

	return shader;
}
