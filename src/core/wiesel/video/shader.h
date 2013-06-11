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
	class ShaderBuilder;
	class ShaderContent;


	/**
	 * @brief A class handling a shader object.
	 */
	class WIESEL_CORE_EXPORT ShaderBase : public virtual SharedObject
	{
	friend class ShaderBuilder;

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
		 * @brief A single entry of a \ref DataSource for this shader.
		 */
		struct DataSourceEntry {
			ref<DataSource>						source;
		};


		/**
		 * @brief Entry of a constant buffer within this shader.
		 */
		struct ConstantBufferTplEntry {
			std::string							name;
			uint16_t							context;
			ref<ShaderConstantBufferTemplate>	buffer_template;
		};


		/// Alias type for an indiced list of attribute names
		typedef std::vector<std::string>				AttributeNamesByIndex;

		/// Alias type for a list of attributes
		typedef std::vector<AttributeNamesByIndex>		AttributeList;

		/// Alias type for a list of sources
		typedef std::map< std::string,DataSourceEntry>	SourcesList;

		/// Alias type for a list of constant buffer templates
		typedef std::vector<ConstantBufferTplEntry>		ConstantBufferTplList;

	protected:
		ShaderBase();

	public:
		virtual ~ShaderBase();

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
		 * @brief Get the name of a shader attribute.
		 */
		std::string getAttributeName(Attribute attr, uint8_t index) const;

		/**
		 * @brief Get the list of all attributes assigned to this shader.
		 */
		const AttributeList* getAttributes() const;

		/**
		 * @brief Find a constant buffer template with the given name, which is assigned to this shader.
		 * @param name	The name of the requested constant buffer.
		 * @return the requested constant buffer, or \c NULL, when no buffer was found.
		 */
		const ShaderConstantBufferTemplate *findConstantBufferTemplate(const std::string &name) const;

		/**
		 * @brief Get the list of all constant buffers, assigned to this shader.
		 */
		const ConstantBufferTplList *getConstantBufferTemplates() const;

		/**
		 * @brief Get the assigned ShaderConstantBufferTemplate for the shader's projection matrix, if any.
		 */
		inline ShaderConstantBufferTemplate* getProjectionMatrixConstantBufferTemplate() {
			return constant_buffer_template_projection;
		}

		/**
		 * @brief Get the assigned ShaderConstantBufferTemplate for the shader's modelview matrix, if any.
		 */
		inline ShaderConstantBufferTemplate* getModelviewMatrixConstantBufferTemplate() {
			return constant_buffer_template_modelview;
		}

	// attributes
	protected:
		/// list of all data sources of this shader
		SourcesList					sources;

		/// A list of attributes according to this shader
		AttributeList				attributes;

		/// A list of all constant buffers assigned to this shader.
		ConstantBufferTplList		constant_buffers;

	protected:
		ref<ShaderConstantBufferTemplate>		constant_buffer_template_projection;
		ref<ShaderConstantBufferTemplate>		constant_buffer_template_modelview;
	};




	/**
	 * @brief A class handling a shader object.
	 */
	class WIESEL_CORE_EXPORT Shader :
			public TDeviceResource<Screen,ShaderContent>,
			public ShaderBase
	{
	friend class ShaderBuilder;

	private:
		Shader();

	public:
		virtual ~Shader();

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();
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
		virtual bool assignShaderConstantBuffer(const ShaderConstantBufferTemplate *buffer_template, ShaderConstantBufferContent *buffer_content) = 0;

	private:
		Shader*		shader;
	};

}
}

#endif /* __WIESEL_VIDEO_SHADER_H__ */
