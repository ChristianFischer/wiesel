/*
 * Shader.cpp
 *
 *  Created on: 28.03.2012
 *      Author: Baldur
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

Shader::~Shader() {
	// shader should be released before deleting the object
	assert(shader == 0);

	if (shader) {
		release();
	}

	return;
}


Shader *Shader::compile(const string &source, ShaderType type) {
	assert(type == ShaderType_VertexShader || type == ShaderType_FragmentShader);

	const char *pSource = source.c_str();
	GLuint shader   = 0;
	GLint  compiled = 0;

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
		return NULL;
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
		Shader *sh = new Shader();
		sh->shader = shader;
		return sh;
	}

	return NULL;
}


void Shader::release() {
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
	return;
}


ShaderProgram::~ShaderProgram() {
	// shader program should be released before deleting the object
	assert(program == 0);

	if (program) {
		release();
	}

	return;
}


bool ShaderProgram::attach(Shader *shader) {
	if (shader) {
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

				break;
			}

			case ShaderType_FragmentShader: {
				break;
			}
		}
	}

	CHECK_GL_ERROR;

	return;
}


void ShaderProgram::release() {
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
