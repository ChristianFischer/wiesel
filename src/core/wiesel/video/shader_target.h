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
#ifndef __WIESEL_VIDEO_SHADER_TARGET_H__
#define	__WIESEL_VIDEO_SHADER_TARGET_H__

#include "wiesel/wiesel-core.def"

#include <wiesel/util/shared_object.h>

#include "shader.h"

#include <string>


namespace wiesel {
namespace video {


	class RenderContext;


	/**
	 * @brief This is the baseclass for any class which uses shaders.
	 */
	class WIESEL_CORE_EXPORT ShaderTarget : public virtual SharedObject
	{
	public:
		ShaderTarget();
		virtual ~ShaderTarget();

	public:
		/**
		 * @brief Set the Shader to be used for rendering.
		 */
		void setShader(Shader *shader);

		/**
		 * @brief Get the currently used shader.
		 */
		inline Shader *getShader() {
			return shader;
		}


	public:
		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param type		The attribute's type.
		 * @param elements	The number of elements within the parameter, to use an array of values.
		 * @param pValue	Pointer to the value, which will be delivered to the shader.
		 */
		void setShaderValue(const std::string &name, Shader::ValueType type, size_t elements, const void *pValue);

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param i			The integer value for the shader attribute.
		 */
		void setShaderValue(const std::string &name, int32_t i);

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param f			The float value for the shader attribute.
		 */
		void setShaderValue(const std::string &name, float f);

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param v			The vector value for the shader attribute.
		 */
		void setShaderValue(const std::string &name, const vector2d &v);

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param v			The vector value for the shader attribute.
		 */
		void setShaderValue(const std::string &name, const vector3d &v);

		/**
		 * @brief Set the uniform attribute for the current shader.
		 * @param name		The unique name of the shader attribute.
		 * @param m			The matrix value for the shader attribute.
		 */
		void setShaderValue(const std::string &name, const matrix4x4 &m);


		/**
		 * @brief Applies the stored shader attributes to the given \ref RenderContext.
		 */
		void applyShaderConfigTo(RenderContext *rc);

	private:
		struct entry
		{
		private:
			entry();
			
		public:
			entry(Shader::ValueType type, size_t elements);
			~entry();
			
			bool match(Shader::ValueType type, size_t elements);
			void set(const void *pValue);

			Shader::ValueType	type;
			size_t				elements;
			size_t				data_size;
			char*				data;
		};

	private:
		typedef std::map<std::string,entry*> ValueMap;

		Shader*		shader;
		ValueMap	values;
	};

} /* namespace video */
} /* namespace wiesel */
#endif	/* __WIESEL_VIDEO_SHADER_TARGET_H__ */
