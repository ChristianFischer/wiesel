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
#include <wiesel/device_resource.h>
#include <inttypes.h>
#include <string>
#include <vector>
#include <map>
#include <set>


namespace wiesel {
namespace video {


	class Screen;
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

		/**
		 * @brief A list of various shader attributes.
		 */
		enum Attribute {
			ProjectionMatrix,
			ModelviewMatrix,

			VertexPosition,
			VertexNormal,
			VertexColor,
			VertexTextureCoordinate,

			Texture,
		};

		/**
		 * @brief A list of valid types for shader attributes.
		 */
		enum ValueType {
			TypeInt32,

			TypeFloat,

			TypeVector2f,
			TypeVector3f,
			TypeVector4f,

			TypeMatrix4x4f,
		};


		/// Alias type for an indiced list of attribute names
		typedef std::vector<std::string>				AttributeNamesByIndex;

		/// Alias type for a list of attributes
		typedef std::vector<AttributeNamesByIndex>		AttributeList;

		/// Alias type for a list of sources
		typedef std::map<std::string,DataSource*>		SourcesList;

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
		 * @brief Add the name of a new uniform attribute.
		 */
		void addUniform(const std::string &name);

		/**
		 * @brief Get the list of all uniform attributes assigned to this shader.
		 */
		const std::set<std::string>* getUniforms() const;

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

		/// names of custom uniform attributes to be set from user code
		std::set<std::string>		uniform_attributes;
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
		/// set the modelview matrix for the current shader
		virtual bool setModelviewMatrix(const matrix4x4 &matrix) = 0;

		/// set the projection matrix for the current shader
		virtual bool setProjectionMatrix(const matrix4x4 &matrix) = 0;

		/// set a uniform shader value, see ShaderTarget::setShaderValue
		virtual bool setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, void *pValue) = 0;

	private:
		Shader*		shader;
	};

}
}

#endif /* __WIESEL_VIDEO_SHADER_H__ */
