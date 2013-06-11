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
#include "gl_shader_content.h"
#include "gl_vertexbuffer_content.h"

#include <wiesel/util/log.h>

#include <sstream>

using namespace wiesel;
using namespace wiesel::video;
using namespace wiesel::video::gl;


GlShaderContent::GlShaderContent(Shader *shader) : ShaderContent(shader) {
	this->program_handle = 0;
	return;
}


GlShaderContent::~GlShaderContent() {
	if (program_handle) {
		releaseShader();
	}

	return;
}


static GLuint compile(GLenum type, DataSource *source) {
	DataBuffer *src_buffer = source->getDataBuffer();
	const char *src_string = src_buffer->getDataAsCharPtr();
	GLint  src_length      = src_buffer->getSize();
	GLint  compiled = 0;
	GLuint shader = 0;

	// check, if this source is a FileDataSource
	FileDataSource *file_data_source = dynamic_cast<FileDataSource*>(source);

	// sources data
	std::vector<const GLchar*>	list_sources;
	std::vector<GLint>			list_sources_length;
	std::string					source_line_string;

	// when the source comes from a file, we're adding a line-hint
	// before the source to get the filename included in our error messages
	if (file_data_source) {
		std::stringstream ss;
		ss << "#line 0 ";
		ss << '"';
		ss << file_data_source->getFile()->getFullPath();
		ss << '"';
		ss << std::endl;

		// prevent this from becoming out of scope
		source_line_string = ss.str();

		list_sources.push_back(source_line_string.c_str());
		list_sources_length.push_back(source_line_string.length());
	}

	// add the actual shader code
	list_sources.push_back(src_string);
	list_sources_length.push_back(src_length);

	// create the shader object
	shader = glCreateShader(type);

	// assign the shader source
	glShaderSource(shader, list_sources.size(), list_sources.data(), list_sources_length.data());
	CHECK_GL_ERROR;

	// compile the shader
	glCompileShader(shader);
	CHECK_GL_ERROR;

	// check, if the shader was compiled
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR;

	// always check the info log to see warning messages
	GLint infoLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	if (!compiled || infoLen > 1) {
		LogLevel loglevel = compiled ? LogLevel_Warning : LogLevel_Error;

		if (file_data_source) {
			logmsg(loglevel, WIESEL_GL_LOG_TAG,
					"In shader %s",
					file_data_source->getFile()->getFullPath().c_str()
			);
		}
		else {
			// print the shader source, which has failed
			logmsg(loglevel, WIESEL_GL_LOG_TAG,
					"Shader code:\n%s",
					std::string(src_string, src_length).c_str()
			);
		}

		// get the error log
		if (infoLen > 1) {
			char *buffer = new char[infoLen];
			GLint length_read = 0;
			glGetShaderInfoLog(shader, infoLen, &length_read, buffer);
			logmsg(loglevel, WIESEL_GL_LOG_TAG, "%s", buffer);
			delete buffer;
		}
		else {
			logmsg(loglevel, WIESEL_GL_LOG_TAG, "unknown error.");
		}
	}

	if (!compiled) {
		glDeleteShader(shader);
		shader = 0;
	}

	CHECK_GL_ERROR;

	return shader;
}


static bool attach(GLuint program, GLuint shader) {
	assert(program);
	assert(shader);

	if (shader) {
		if (program) {
			glAttachShader(program, shader);

			// mark for deletion after attaching
			glDeleteShader(shader);

			return true;
		}
		else {
			// just delete, if not attached
			glDeleteShader(shader);
		}
	}

	return false;
}


static GLuint compile_and_attach(GLuint program, GLenum type, DataSource *source) {
	GLuint shader = compile(type, source);

	if (shader) {
		return attach(program, shader);
	}

	return false;
}


static bool link(GLuint program) {
	assert(program);

	if (program != 0) {
		// link the program
		glLinkProgram(program);

		// check link status
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (linkStatus != GL_TRUE || infoLen > 1) {
			LogLevel loglevel = linkStatus == GL_TRUE ? LogLevel_Warning : LogLevel_Error;

			if (infoLen) {
				char* buffer = new char[infoLen];
				GLint length_read = 0;
				glGetProgramInfoLog(program, infoLen, &length_read, buffer);

				logmsg(
						loglevel,
						WIESEL_GL_LOG_TAG,
						"On linking shader program:\n%s\n",
						buffer
				);

				delete buffer;
			}
			else {
				logmsg(
						loglevel,
						WIESEL_GL_LOG_TAG,
						"Could not link program for unknown reason."
				);
			}
		}

		if (linkStatus != GL_TRUE) {
			return false;
		}

		CHECK_GL_ERROR;
	}

	return true;
}


GlShaderContent *GlShaderContent::createContentFor(Shader* shader) {
	if (shader->getSources()->size() > 0) {
		GlShaderContent *gl_shader = new GlShaderContent(shader);
		gl_shader->initializeShader();

		DataSource *source;
		bool success = true;

		source = shader->getSource(Shader::GLSL_VERTEX_SHADER);
		if (source) {
			success &= compile_and_attach(gl_shader->program_handle, GL_VERTEX_SHADER, source);
			assert(success);
		}

		source = shader->getSource(Shader::GLSL_FRAGMENT_SHADER);
		if (source) {
			success &= compile_and_attach(gl_shader->program_handle, GL_FRAGMENT_SHADER, source);
			assert(success);
		}

		// link the program
		success &= link(gl_shader->program_handle);
		assert(success);

		// when failed, delete the shader
		if (!success) {
			// TODO: delete gl_shader;
			return NULL;
		}

		// try to get handles to all attributes
		gl_shader->bindAttributes();

		return gl_shader;
	}

	return NULL;
}


void GlShaderContent::initializeShader() {
	assert(program_handle == 0);
	if (program_handle == 0) {
		program_handle = glCreateProgram();
	}

	return;
}


void GlShaderContent::releaseShader() {
	if (program_handle == 0) {
		glDeleteProgram(program_handle);
	}

	return;
}


void GlShaderContent::bindAttributes() {
	const Shader::AttributeList*			attributes			= getShader()->getAttributes();
	const Shader::ConstantBufferTplList*	buffer_templates	= getShader()->getConstantBufferTemplates();

	// prepare the lists
	uniform_attributes.clear();
	attribute_handles.clear();
	attribute_handles.resize(attributes->size());

	// get all attribute handles
	for(int attr=attributes->size(); --attr>=0;) {
		const Shader::AttributeNamesByIndex *attr_names = &(attributes->at(attr));

		// prepare the list for all indexed attributes
		attribute_handles[attr].clear();
		attribute_handles[attr].resize(attr_names->size(), -1);

		for(int index=attr_names->size(); --index>=0;) {
			const std::string &name = attr_names->at(index);
			GLint handle = -1;

			switch(static_cast<Shader::Attribute>(attr)) {
				case Shader::Texture: {
					handle = glGetUniformLocation(program_handle, name.c_str());
					break;
				}

				case Shader::VertexPosition:
				case Shader::VertexNormal:
				case Shader::VertexColor:
				case Shader::VertexTextureCoordinate: {
					handle = glGetAttribLocation(program_handle, name.c_str());
					break;
				}
			}

			CHECK_GL_ERROR;

			if (handle == -1) {
				Log::warn << "Could not find member '" << name << "' in current shader." << std::endl;
			}

			assert(handle != -1);
			attribute_handles[attr][index] = handle;
		}
	}

	// get uniform handles from all constant buffers
	for(Shader::ConstantBufferTplList::const_iterator
			tpl_it  = buffer_templates->begin();
			tpl_it != buffer_templates->end();
			tpl_it++
	) {
		const ShaderConstantBufferTemplate *buffer_template = tpl_it->buffer_template;
		const ShaderConstantBufferTemplate::EntryList *buffer_template_entries = buffer_template->getEntries();

		for(ShaderConstantBufferTemplate::EntryList::const_iterator
				e_it  = buffer_template_entries->begin();
				e_it != buffer_template_entries->end();
				e_it++
		) {
			GLint handle = glGetUniformLocation(program_handle, e_it->name.c_str());
		//	assert(handle != -1);
			CHECK_GL_ERROR;

			if (handle != -1) {
				uniform_attributes[e_it->name] = handle;
			}
		}
	}

	return;
}


GLint GlShaderContent::getAttribHandle(Shader::Attribute attr, uint8_t index) const {
	if (attribute_handles.size() >= attr) {
		const AttributeHandlesByIndex* attrib_handles_by_index = &(attribute_handles[attr]);

		if (attrib_handles_by_index->size() >= index) {
			return (*attrib_handles_by_index)[index];
		}
	}

	return -1;
}


bool GlShaderContent::assignShaderConstantBuffer(const ShaderConstantBufferTemplate *buffer_template, ShaderConstantBufferContent* buffer_content) {
	if (buffer_template) {
		ShaderConstantBuffer::version_t new_version = buffer_content->getShaderConstantBuffer()->getChangeVersion();
		BufferEntryMap::iterator entry = buffer_entries.find(buffer_template);

		// create a new entry, if not available
		if (entry == buffer_entries.end()) {
			BufferEntry buffer_entry;
			buffer_entry.buffer  = buffer_content->getShaderConstantBuffer();
			buffer_entry.version = 0;

			// store uniform informations for each uniform, which belongs to this buffer
			const ShaderConstantBufferTemplate::EntryList *entries = buffer_template->getEntries();
			for(ShaderConstantBufferTemplate::EntryList::const_iterator it_unif=entries->begin(); it_unif!=entries->end(); it_unif++) {
				std::map<std::string,GLint>::iterator it_handle = uniform_attributes.find(it_unif->name);

				if (it_handle != uniform_attributes.end()) {
					UniformEntry uniform_entry;
					uniform_entry.entry  = &(*it_unif);
					uniform_entry.handle = it_handle->second;
					buffer_entry.buffer_uniforms.push_back(uniform_entry);
				}
			}

			buffer_entries[buffer_template] = buffer_entry;

			entry = buffer_entries.find(buffer_template);
			assert(entry != buffer_entries.end());
		}

		// check if the content has changed
		if (
				entry->second.version != new_version
			||	entry->second.buffer  != buffer_content->getShaderConstantBuffer()
		) {
			entry->second.version = new_version;
			entry->second.buffer  = buffer_content->getShaderConstantBuffer();

			const UniformEntryList *uniform_entries = &(entry->second.buffer_uniforms);
			const ShaderConstantBuffer::data_t data_ptr = buffer_content->getShaderConstantBuffer()->getDataPtr();

			for(UniformEntryList::const_iterator it=uniform_entries->begin(); it!=uniform_entries->end(); it++) {
				if (it->handle != -1) {
					bool success = setShaderValue(
										it->handle,
										it->entry->type,
										it->entry->elements,
										data_ptr + it->entry->offset
					);

					assert(success);
				}
			}
		}

		return true;
	}

	return false;
}


bool GlShaderContent::setShaderValue(const std::string &name, ValueType type, size_t elements, void *pValue) {
	std::map<std::string,GLint>::iterator it = uniform_attributes.find(name);
	if (it != uniform_attributes.end()) {
		GLint attrib_handle = it->second;
		return setShaderValue(attrib_handle, type, elements, pValue);
	}

	return false;
}

bool GlShaderContent::setShaderValue(GLint attrib_handle, ValueType type, size_t elements, void *pValue) {
	if (attrib_handle != -1) {
		switch(type) {
			case TypeInt32: {
				if (elements == 1) {
					glUniform1i(attrib_handle, *(reinterpret_cast<GLint*>(pValue)));
				}
				else {
					glUniform1iv(attrib_handle, elements, reinterpret_cast<GLint*>(pValue));
				}

				break;
			}

			case TypeFloat: {
				if (elements == 1) {
					glUniform1f(attrib_handle, *(reinterpret_cast<GLfloat*>(pValue)));
				}
				else {
					glUniform1fv(attrib_handle, elements, reinterpret_cast<GLfloat*>(pValue));
				}

				break;
			}

			case TypeVector2f: {
				glUniform2fv(attrib_handle, elements, reinterpret_cast<GLfloat*>(pValue));
				break;
			}

			case TypeVector3f: {
				glUniform3fv(attrib_handle, elements, reinterpret_cast<GLfloat*>(pValue));
				break;
			}

			case TypeVector4f: {
				glUniform4fv(attrib_handle, elements, reinterpret_cast<GLfloat*>(pValue));
				break;
			}

			case TypeMatrix4x4f: {
				glUniformMatrix4fv(attrib_handle, elements, false, reinterpret_cast<GLfloat*>(pValue));
				break;
			}

			default: {
				return false;
			}
		}

		CHECK_GL_ERROR;

		return true;
	}

	return false;
}

