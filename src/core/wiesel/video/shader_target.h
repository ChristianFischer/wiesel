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

#include "shader_constantbuffer.h"
#include "shader.h"
#include "types.h"

#include <string>


namespace wiesel {
namespace video {


	class RenderContext;


	/**
	 * @brief This is the baseclass for any class which uses shaders.
	 */
	class WIESEL_CORE_EXPORT ShaderTarget : public ShaderConstantBufferWriter
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

	// ConstantBufferSetter
	protected:
		virtual bool doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue);

	public:
		/**
		 * @brief Applies the stored shader attributes to the given \ref RenderContext.
		 */
		void applyShaderConfigTo(RenderContext *rc);

	private:
		typedef std::map<std::string,ShaderConstantBuffer*> BufferMap;

		Shader*		shader;
		BufferMap	buffers;
	};

} /* namespace video */
} /* namespace wiesel */
#endif	/* __WIESEL_VIDEO_SHADER_TARGET_H__ */
