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
#include "shader.h"

#include "wiesel/gl/gl.h"
#include "wiesel/util/log.h"
#include <assert.h>

using namespace wiesel;
using namespace std;


Shader::Shader() {
	return;
}


Shader::Shader(const std::string &source, ShaderType type) {
	assert(type == ShaderType_VertexShader || type == ShaderType_FragmentShader);

	DataBuffer *buffer = ExclusiveDataBuffer::createCopyOf(source);
	this->shader = 0;
	this->type   = type;
	this->source = new BufferDataSource(buffer);
	this->source->retain();

	return;
}


Shader::Shader(DataSource *source, ShaderType type) {
	assert(type == ShaderType_VertexShader || type == ShaderType_FragmentShader);

	this->shader = 0;
	this->type   = type;
	this->source = source;
	this->source->retain();

	return;
}


Shader::~Shader() {
	if (source) {
		source->release();
	}

	if (shader) {
		release_shader();
	}

	return;
}


Shader *Shader::compile(const string &source, ShaderType type) {
	Shader *shader = new Shader(source, type);
	if (shader->compile()) {
		return shader;
	}
	else {
		delete shader;
	}

	return NULL;
}


Shader *Shader::compile(DataSource *source, ShaderType type) {
	Shader *shader = new Shader(source, type);
	if (shader->compile()) {
		return shader;
	}
	else {
		delete shader;
	}

	return NULL;
}


bool Shader::compile() {
	assert(type == ShaderType_VertexShader || type == ShaderType_FragmentShader);

	// release previous shader, if any
	release_shader();

	const char *pSource = source->getDataBuffer()->getDataAsCharPtr();
	GLint  compiled = 0;
	shader = 0;

	switch(type) {
		case ShaderType_VertexShader: {
			shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		}

		case ShaderType_FragmentShader: {
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}
	}

	if (shader == 0) {
		return false;
	}

	// assign the shader source
	glShaderSource(shader, 1, &pSource, NULL);
	CHECK_GL_ERROR;

	// compile the shader
	glCompileShader(shader);
	CHECK_GL_ERROR;

	// check, if the shader was compiled
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR;

	if (!compiled) {
		// get the info message, which caused the compile to fail
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Could not compile shader:\n%s", pSource);

		if (infoLen) {
			char *buffer = new char[infoLen];

			if (buffer) {
				GLint length_read = 0;
				glGetShaderInfoLog(shader, infoLen, &length_read, buffer);
				logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "%s", buffer);
				delete buffer;
			}
			else {
				logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "unknown reason.");
			}
		}

		glDeleteShader(shader);
		shader = 0;
	}

	CHECK_GL_ERROR;

	if (shader) {
		return true;
	}

	return false;
}


void Shader::release_shader() {
	if (shader) {
		glDeleteShader(shader);
		shader = 0;
	}

	return;
}






ShaderProgram::ShaderProgram()
 :	program(0),
  	need_link(false)
{
	attrib_handle_vertex_position		= -1;
	attrib_handle_vertex_normal			= -1;
	attrib_handle_vertex_color			= -1;
	return;
}


ShaderProgram::~ShaderProgram() {
	if (program) {
		release_shader();
	}

	for(vector<Shader*>::iterator it=shaders.begin(); it!=shaders.end(); it++) {
		(*it)->release();
	}

	return;
}


bool ShaderProgram::attach(Shader *shader) {
	if (shader) {
		shader->retain();
		shaders.push_back(shader);
		need_link = true;

		return true;
	}

	return false;
}


bool ShaderProgram::link() {
	if (need_link) {
		// create the program, if neccessary
		if (program == 0) {
			program = glCreateProgram();
		}

		// fail, if program could not be created.
		if (program == 0) {
			logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "could not create shader program");
			return false;
		}

		// attach each shader
		for(vector<Shader*>::iterator it=shaders.begin(); it!=shaders.end(); it++) {
			glAttachShader(program, (*it)->getGlShader());
		}

		// link the program
		glLinkProgram(program);

		// check link status
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		if (linkStatus != GL_TRUE) {
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			if (length) {
				char* buffer = new char[length];
				if (buffer) {
					GLint length_read = 0;
					glGetProgramInfoLog(program, length, &length_read, buffer);
					logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Could not link program:\n%s\n", buffer);
					delete buffer;
				}
				else {
					logmsg(LogLevel_Error, WIESEL_GL_LOG_TAG, "Could not link program for unknown reason.");
				}
			}

			glDeleteProgram(program);
			program = 0;
		}

		CHECK_GL_ERROR;

		// bind all configured attributes
		bindAttributes();

		need_link = false;
	}

	return true;
}


void ShaderProgram::bindAttributes() {
	// get all attribute handles
	for(vector<Shader*>::iterator it=shaders.begin(); it!=shaders.end(); it++) {
		Shader *shader = *it;

		switch(shader->getType()) {
			case ShaderType_VertexShader: {
				if (shader->attrib_vertex_position.empty() == false) {
					attrib_handle_vertex_position = glGetAttribLocation(program, shader->attrib_vertex_position.c_str());
					CHECK_GL_ERROR;
					assert(attrib_handle_vertex_position != -1);
				}

				if (shader->attrib_vertex_normal.empty() == false) {
					attrib_handle_vertex_normal = glGetAttribLocation(program, shader->attrib_vertex_normal.c_str());
					CHECK_GL_ERROR;
					assert(attrib_handle_vertex_position != -1);
				}

				if (shader->attrib_vertex_color.empty() == false) {
					attrib_handle_vertex_color = glGetAttribLocation(program, shader->attrib_vertex_color.c_str());
					CHECK_GL_ERROR;
					assert(attrib_handle_vertex_position != -1);
				}

				attrib_handle_vertex_texcoords.clear();
				for(vector<string>::const_iterator it=shader->attrib_vertex_texcoords.begin(); it!=shader->attrib_vertex_texcoords.end(); it++) {
					GLuint handle = glGetAttribLocation(program, it->c_str());
					attrib_handle_vertex_texcoords.push_back(handle);
					CHECK_GL_ERROR;
				}

				break;
			}

			case ShaderType_FragmentShader: {
				attrib_handle_vertex_textures.clear();
				for(vector<string>::const_iterator it=shader->attrib_vertex_textures.begin(); it!=shader->attrib_vertex_textures.end(); it++) {
					GLuint handle = glGetUniformLocation(program, it->c_str());
					attrib_handle_vertex_textures.push_back(handle);
					CHECK_GL_ERROR;
				}

				break;
			}
		}
	}

	CHECK_GL_ERROR;

	return;
}


void ShaderProgram::release_shader() {
	glDeleteProgram(program);
	return;
}


void ShaderProgram::bind() const {
	assert(need_link == false);

	if (program) {
		glUseProgram(program);
		CHECK_GL_ERROR;
	}

	return;
}


void ShaderProgram::unbind() const {
	glUseProgram(0);
	CHECK_GL_ERROR;
	return;
}
