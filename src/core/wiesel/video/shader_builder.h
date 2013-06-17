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
#ifndef __WIESEL_VIDEO_SHADER_BUILDER_H__
#define __WIESEL_VIDEO_SHADER_BUILDER_H__

#include <wiesel/wiesel-core.def>

#include "shader.h"


namespace wiesel {

	class File;
	class FileSystem;

namespace video {


	/**
	 * @brief An abstract baseclass for \ref Shader.
	 */
	class WIESEL_CORE_EXPORT ShaderBuilder : public ShaderBase
	{
	public:
		ShaderBuilder();

		virtual ~ShaderBuilder();

	public:
		/**
		 * @brief Set a specific source.
		 * @param name		The source name. Valid values are:
		 *					<ul><li>\ref GLSL_VERTEX_SHADER</li>
		 *					    <li>\ref GLSL_FRAGMENT_SHADER</li>
		 *					</ul>
		 * @param source	The source object to store.
		 */
		void setSource(const std::string &name, DataSource *source);

		/**
		 * @brief Set the shader's sources by using conventional filenames.
		 * Given a filename, the builder adds sources for the following files, if existent:
		 * <ul><li><i>filename</i>.glsl.vert (GLSL vertex shader)</li>
		 *     <li><i>filename</i>.glsl.frag (GLSL fragment shader)</li>
		 *     <li><i>filename</i>.hlsl.vert (HLSL vertex shader)</li>
		 *     <li><i>filename</i>.hlsl.frag (HLSL fragment shader)</li>
		 * </ul>
		 * @param fs		The filesystem where to get the sources from.
		 * @param filename	The filename of the shaders without any file extensions,
		 *					but including any subdirectories within the given filesystem.
		 * @return \c true, when at least one source was found, \c false otherwise.
		 */
		bool setSourcesFromFiles(FileSystem *fs, const std::string& filename);

		/**
		 * @brief Set a preprocessor constant to a new value.
		 * The constant will be added, if it does not exist. When a constant with the name
		 * already exists, the value will be updated.
		 * All constants will be added to the source in the same order, as they were added.
		 * @param name		The constant's name.
		 * @param value		The constant's value.
		 *					When omitted, the constant has no value, but still exists.
		 */
		void setPreprocessorConstant(const std::string& name, const std::string& value=std::string());

		/**
		 * @brief Removes a given preprocessor constant from this shader.
		 * @param name		The constant's name.
		 */
		void undefPreprocessorConstant(const std::string& name);

		/**
		 * @brief Sets the name of a shader attribute.
		 * @param attr		The type of the attribute to set.
		 * @param index		The index of the attribute to set, if an attribute has
		 *					multiple instances (like texture coordinates)
		 * @param name		The attribute's name to set.
		 * @return \c true, when the attribute was successfully set, \c false otherwise.
		 */
		bool setAttributeName(Attribute attr, uint8_t index, const std::string &name);

		/**
		 * @brief Sets the default name of a shader attribute.
		 * @param attr		The type of the attribute to set.
		 * @param index		The index of the attribute to set, if an attribute has
		 *					multiple instances (like texture coordinates)
		 * @return \c true, when the attribute was successfully set, \c false otherwise.
		 */
		bool setDefaultAttributeName(Attribute attr, uint8_t index);

		/**
		 * @brief Adds a new constant buffer template to this shader.
		 * @param name				The name of the new constant buffer.
		 * @param context			One or more flags of \ref Context to determine,
		 *							which shader stages are using this buffer.
		 * @param buffer_template	The constant buffer template, which
		 * @return \c true on success, \c false otherwise.
		 */
		bool addConstantBuffer(const std::string &name, uint16_t context, ShaderConstantBufferTemplate *buffer_template);

		/**
		 * @brief Adds the default constant buffer containing the modelview matrix.
		 * The according constant buffer can be retrieved via \ref Shaders::getModelviewMatrixBufferTemplate()
		 * @return \c true on success, \c false otherwise.
		 */
		bool addDefaultModelviewMatrixConstantBuffer();

		/**
		 * @brief Adds the default constant buffer containing the projection matrix.
		 * The according constant buffer can be retrieved via \ref Shaders::getProjectionatrixBufferTemplate()
		 * @return \c true on success, \c false otherwise.
		 */
		bool addDefaultProjectionMatrixConstantBuffer();

	public:
		/**
		 * @brief Create a new instance \ref Shader instance,
		 * based on the current settings.
		 */
		Shader* create() const;
	};

}
}

#endif // __WIESEL_VIDEO_SHADER_BUILDER_H__
