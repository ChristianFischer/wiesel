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
#ifndef __WIESEL_VIDEO_SHADER_H__
#define __WIESEL_VIDEO_SHADER_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/math/matrix.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>
#include <wiesel/io/datasource.h>
#include <wiesel/video/screen.h>
#include <wiesel/video/shader_constantbuffer.h>
#include <wiesel/video/types.h>
#include <wiesel/device_resource.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <set>


namespace wiesel {
namespace video {


	class Shader;
	class ShaderContent;


	/**
	 * @brief A class handling a shader object.
	 */
	class WIESEL_CORE_EXPORT Shader : public TDeviceResource<Screen,ShaderContent>
	{
	public:
		static const char* GLSL_VERTEX_SHADER;		//!< Source type for GLSL vertex shaders.
		static const char* GLSL_FRAGMENT_SHADER;	//!< Source type for GLSL fragment shaders.

		static const char* HLSL_VERTEX_SHADER;		//!< Source type for HLSL vertex shaders.
		static const char* HLSL_FRAGMENT_SHADER;	//!< Source type for HLSL fragment shaders.


		/**
		 * @brief A list of various shader attributes.
		 */
		enum Attribute {
			VertexPosition,
			VertexNormal,
			VertexColor,
			VertexTextureCoordinate,

			Texture,
		};


		/**
		 * @brief Enumeration of various shader stages.
		 */
		enum Context {
			Context_VertexShader		= 0x0001,
			Context_FragmentShader		= 0x0002,
		};


		/**
		 * @brief Entry of a constant buffer within this shader.
		 */
		struct ConstantBufferTplEntry {
			std::string						name;
			uint16_t						context;
			ShaderConstantBufferTemplate*	buffer_template;
		};


		/// Alias type for an indiced list of attribute names
		typedef std::vector<std::string>				AttributeNamesByIndex;

		/// Alias type for a list of attributes
		typedef std::vector<AttributeNamesByIndex>		AttributeList;

		/// Alias type for a list of sources
		typedef std::map<std::string,DataSource*>		SourcesList;

		/// Alias type for a list of constant buffer templates
		typedef std::vector<ConstantBufferTplEntry>		ConstantBufferTplList;

	public:
		Shader();

		virtual ~Shader();

		/**
		 * @brief Set a specific source.
		 * @param name		The source name. Valid values are:
		 *			0		<ul><li>\ref GLSL_VERTEX_SHADER</li>
		 *					    <li>\ref GLSL_FRAGMENT_SHADER</li>
		 *					</ul>
		 * @param source	The source object to store.
		 */
		void setSource(const std::string &name, DataSource *source);

		/**
		 * @brief Get a specific source assigned to this shader.
		 * @return
		 */
		DataSource *getSource(const std::string &name);

		/**
		 * @brief Get the list of sources assigned to this shader.
		 */
		const SourcesList *getSources() const;

	public:
		/**
		 * @brief Sets the name of a shader attribute.
		 */
		void setAttributeName(Attribute attr, uint8_t index, const std::string &name);

		/**
		 * @brief Get the name of a shader attribute.
		 */
		std::string getAttributeName(Attribute attr, uint8_t index) const;

		/**
		 * @brief Get the list of all attributes assigned to this shader.
		 */
		const AttributeList* getAttributes() const;

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
		 * @brief Find a constant buffer template with the given name, which is assigned to this shader.
		 * @param name	The name of the requested constant buffer.
		 * @return the requested constant buffer, or \c NULL, when no buffer was found.
		 */
		ShaderConstantBufferTemplate *findConstantBufferTemplate(const std::string &name) const;

		/**
		 * @brief Get the list of all constant buffers, assigned to this shader.
		 */
		const ConstantBufferTplList *getConstantBufferTemplates() const;

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();

	// attributes
	private:
		/// list of all data sources of this shader
		SourcesList					sources;

		/// A list of attributes according to this shader
		AttributeList				attributes;

		/// A list of all constant buffers assigned to this shader.
		ConstantBufferTplList		constant_buffers;
	};




	/**
	 * @brief Handles the device specific part of a Shader.
	 */
	class WIESEL_CORE_EXPORT ShaderContent : public virtual DeviceResourceContent
	{
	private:
		ShaderContent();

	protected:
		ShaderContent(Shader *shader);

	public:
		virtual ~ShaderContent();

	// public getter
	public:
		/// get the assigned shader
		inline Shader *getShader() {
			return shader;
		}

	// setters
	public:
		/// assigns a constant buffer to the current shader instance.
		virtual bool assignShaderConstantBuffer(const std::string &name, ShaderConstantBufferContent *buffer_content) = 0;

	private:
		Shader*		shader;
	};

}
}

#endif /* __WIESEL_VIDEO_SHADER_H__ */
