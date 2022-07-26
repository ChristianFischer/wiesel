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
#ifndef __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_H__
#define __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/video/screen.h>
#include <wiesel/device_resource.h>

#include <wiesel/math/matrix.h>
#include <wiesel/math/vector2d.h>
#include <wiesel/math/vector3d.h>

#include "types.h"

#include <string>
#include <vector>


namespace wiesel {
namespace video {

	class ShaderConstantBuffer;
	class ShaderConstantBufferContent;
	class ShaderConstantBufferTemplate;
	class ShaderConstantBufferTemplateBuilder;


	/**
	 * @brief An abstract baseclass for \ref ShaderConstantBufferTemplate.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBufferTemplateBase : public virtual SharedObject
	{
	public:
		/**
		 * @brief An entry for a constant entry.
		 */
		struct Entry {
		public:
			ValueType		type;
			size_t			elements;
			size_t			offset;
			std::string		name;
		};

		/// Alias type for a list of entries
		typedef std::vector<Entry>		EntryList;

		/// type for indices of the values within this buffer
		typedef signed int index_t;

	protected:
		ShaderConstantBufferTemplateBase();

	public:
		virtual ~ShaderConstantBufferTemplateBase();

	public:
		/**
		 * @brief Find an entry entry matching the given name,
		 * or \c NULL, if no entry with that name was found.
		 */
		const Entry *findEntry(const std::string &name) const;

		/**
		 * @brief Tries to find the index of a specific shader value.
		 * @param name		Name of the variable to get.
		 * @return The index of the requested variable or \c -1, when not found.
		 */
		index_t findShaderValueIndex(const std::string& name) const;

		/**
		 * @brief Returns the list of all entries, contained in this buffer.
		 */
		inline const EntryList* getEntries() const {
			return &entries;
		}

		/**
		 * @brief Get the total size of this constant buffer.
		 */
		inline size_t getSize() const {
			return size;
		}

	protected:
		EntryList						entries;
		size_t							size;
	};




	/**
	 * @brief A class which describes the content of a constant buffer.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBufferTemplate : public ShaderConstantBufferTemplateBase
	{
	friend class ShaderConstantBufferTemplateBuilder;

	private:
		ShaderConstantBufferTemplate();

	public:
		virtual ~ShaderConstantBufferTemplate();

	public:
		/**
		 * @brief Get or create a constant buffer, which can be used
		 * shared between all instances of this template
		 * @return
		 */
		ShaderConstantBuffer* getSharedBuffer();

	private:
		ref<ShaderConstantBuffer>		shared_buffer;
	};





	/**
	 * @brief A helper class which implements various methods to set values of a constant buffer.
	 * The implementing class needs to implement the mothods to actually write the data into
	 * the corresponding buffer.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBufferWriter : public virtual SharedObject
	{
	public:
		/// pointer type
		typedef unsigned char* data_t;

	public:
		ShaderConstantBufferWriter();

		virtual ~ShaderConstantBufferWriter();

	// setter (by name)
	public:
		/**
		 * @brief Set an integer value for the current shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param i			The integer value for the shader attribute.
		 */
		bool setShaderValue(const std::string &name, int32_t i);

		/**
		 * @brief Set a float value for the current shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param f			The float value for the shader attribute.
		 */
		bool setShaderValue(const std::string &name, float f);

		/**
		 * @brief Set a 2D-vector value for the current shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param v			The vector value for the shader attribute.
		 */
		bool setShaderValue(const std::string &name, const vector2d &v);

		/**
		 * @brief Set a 3D-vector value for the current shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param v			The vector value for the shader attribute.
		 */
		bool setShaderValue(const std::string &name, const vector3d &v);

		/**
		 * @brief Set a 4x4 matrix value for the current shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param m			The matrix value for the shader attribute.
		 */
		bool setShaderValue(const std::string &name, const matrix4x4 &m);

		/**
		 * @brief Set a single value of a shader constant buffer.
		 * @param name		The unique name of the shader attribute.
		 * @param type		The value type.
		 * @param elements	The number of elements of this value, to use an array of values.
		 * @param pValue	Pointer to the value, which will be written into the constant buffer.
		 */
		bool setShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue);

	// overridables
	public:
		/**
		 * @brief Get a pointer to the value of the given variable.
		 * @param name		Name of the variable to get.
		 * @param type		Type of the variables.
		 * @param elements	Number of elements of this variable, when this variable is an array.
		 * @return A pointer to the requested variable, or \c NULL, when the variable was not found
		 *			or does not match the given types.
		 */
		virtual const data_t getShaderDataPointer(const std::string &name, ValueType type, size_t elements) const = 0;

	/// overridables
	protected:
		/**
		 * @brief Actually writes a value into the according buffer.
		 * This needs to be implemented in the accoding subclass.
		 * @param name		Name of the variable to be set.
		 * @param type		Type of the variable.
		 * @param elements	Number of elements of this variable, when this variable is an array.
		 * @param pValue	Pointer to the variable's data.
		 * @return \c true on success, \c false otherwise.
		 */
		virtual bool doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue) = 0;
	};




	/**
	 * @brief A constant buffer holds a user defined package of data.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBuffer :
			public TDeviceResource<Screen, ShaderConstantBufferContent>,
			public ShaderConstantBufferWriter
	{
	public:
		/// type for the constant buffer version number
		typedef uint16_t version_t;

		/// type for indices of the values within this buffer
		typedef signed int index_t;

		ShaderConstantBuffer(const ShaderConstantBufferTemplate *buffer_template);
		virtual ~ShaderConstantBuffer();

	private:
		ShaderConstantBuffer();

	public:
		/**
		 * @brief Get the template assigned to this buffer.
		 */
		inline const ShaderConstantBufferTemplate* getTemplate() const {
			return buffer_template;
		}

	// setter (by index))
	public:
		/**
		 * @brief Set an integer value for the current shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param i			The integer value for the shader attribute.
		 */
		bool setShaderValueAt(index_t index, int32_t i);

		/**
		 * @brief Set a float value for the current shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param f			The float value for the shader attribute.
		 */
		bool setShaderValueAt(index_t index, float f);

		/**
		 * @brief Set a 2D-vector value for the current shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param v			The vector value for the shader attribute.
		 */
		bool setShaderValueAt(index_t index, const vector2d &v);

		/**
		 * @brief Set a 3D-vector value for the current shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param v			The vector value for the shader attribute.
		 */
		bool setShaderValueAt(index_t index, const vector3d &v);

		/**
		 * @brief Set a 4x4 matrix value for the current shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param m			The matrix value for the shader attribute.
		 */
		bool setShaderValueAt(index_t index, const matrix4x4 &m);

		/**
		 * @brief Set a single value of a shader constant buffer.
		 * @param index		The index of the variable to set.
		 * @param type		The value type.
		 * @param elements	The number of elements of this value, to use an array of values.
		 * @param pValue	Pointer to the value, which will be written into the constant buffer.
		 */
		bool setShaderValueAt(index_t index, ValueType type, size_t elements, const void *pValue);

	// data access
	public:
		/**
		 * @brief Writes raw data to a given offset.
		 * The buffer needs to be large enough to store the whole data block,
		 * otherwise the operation will fail.
		 * @return \c true on success, \c false when the data could not be written.
		 */
		bool writeDataAtOffset(size_t offset, const void *pValue, size_t size);

		/**
		 * @brief Get the pointer to the constant buffer's data.
		 * The data is meant to be read-only and should be written only by using
		 * the functions provided by the ConstantBuffer class.
		 */
		inline const data_t getDataPtr() const {
			return data;
		}

		/**
		 * @brief Get the current change version.
		 * This value starts with 1 and increments each time, the buffer's content
		 * has changed. A shader can store this value and compare it with a buffer's
		 * current change version to check, if it's content has been updated.
		 * The change version will start with 1 and also resets to 1, when reaches it's
		 * maximum. It will never reach zero, so this would be the ideal initial reference
		 * value for shaders.
		 */
		inline version_t getChangeVersion() const {
			return change_ver;
		}

	// ShaderConstantBufferWriter
	public:
		/**
		 * @brief Get a pointer to the value of a variable by it's index.
		 * @param index		The index of the requested variable.
		 * @return A pointer to the requested variable, or \c NULL, when the variable was not found.
		 */
		virtual const data_t getShaderDataPointerAt(ShaderConstantBufferTemplate::index_t index) const;

		virtual const data_t getShaderDataPointer(const std::string &name, ValueType type, size_t elements) const;

	// ShaderConstantBufferWriter
	protected:
		virtual bool doSetShaderValue(const std::string &name, ValueType type, size_t elements, const void *pValue);

	// DeviceResource implementation
	protected:
		virtual bool doLoadContent();
		virtual bool doUnloadContent();

	private:
		/**
		 * @brief Increments the change version so other objects can check if this buffer was changed.
		 */
		void incrementChangeVersion();

	private:
		const ShaderConstantBufferTemplate*	buffer_template;
		data_t								data;
		version_t							change_ver;
	};




	/**
	 * @brief Handles the device specific part of a index buffer.
	 */
	class WIESEL_CORE_EXPORT ShaderConstantBufferContent : public DeviceResourceContent
	{
	private:
		ShaderConstantBufferContent();

	protected:
		ShaderConstantBufferContent(ShaderConstantBuffer *shader_constant_buffer);

	public:
		virtual ~ShaderConstantBufferContent();

	// public getter
	public:
		/// get the assigned texture
		inline ShaderConstantBuffer *getShaderConstantBuffer() {
			return shader_constant_buffer;
		}

	private:
		ShaderConstantBuffer*		shader_constant_buffer;
	};

} /* namespace video */
} /* namespace wiesel */
#endif /* __WIESEL_VIDEO_SHADER_CONSTANTBUFFER_H__ */
