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
#ifndef __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_BUILDER_H__
#define __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_BUILDER_H__

#include <wiesel/wiesel-core.def>

#include "shader_constantbuffer.h"


namespace wiesel {
namespace video {

	/**
	 * @brief A class which describes the content of a constant buffer.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBufferTemplateBuilder : public ShaderConstantBufferTemplateBase
	{
	public:
		ShaderConstantBufferTemplateBuilder();

		virtual ~ShaderConstantBufferTemplateBuilder();

	public:
		/**
		 * @brief Adds a new entry for this constant buffer.
		 * @param type		The type of the new entry.
		 * @param elements	The number of elements, which belongs to this entry.
		 *					This can be used to create an array of values.
		 * @param name		The name of the new entry.
		 * @return \c true, when the uniform was added successfully.
		 */
		bool addEntry(ValueType type, size_t elements, const std::string &name);

		/**
		 * @brief Create a new instance of \ref ShaderConstantBufferTemplate,
		 * based on the current settings.
		 */
		ShaderConstantBufferTemplate* create() const;
	};

} /* namespace video */
} /* namespace wiesel */

#endif /* __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_BUILDER_H__ */
