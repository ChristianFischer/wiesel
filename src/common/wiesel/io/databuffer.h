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
#ifndef __WIESEL_IO_DATABUFFER_H__
#define __WIESEL_IO_DATABUFFER_H__

#include "wiesel/util/shared_object.h"
#include <string>


namespace wiesel {

	/**
	 * @brief Wrapper object for any kind of data buffers.
	 */
	class DataBuffer : public virtual SharedObject
	{
	protected:
		DataBuffer();

	public:
		typedef const unsigned char* data_t;

		virtual ~DataBuffer();

		/**
		 * @brief Receive a pointer to the data buffer.
		 * It depends on the buffer's implementation, if the content
		 * is accessible after the buffer's release.
		 * The application should not use this pointer after the
		 * object was released.
		 */
		virtual data_t getData() const = 0;

		/**
		 * @brief Get the size of the data stored in the buffer.
		 */
		virtual size_t getSize() const = 0;

		/**
		 * @brief Receive a pointer to the data buffer as pointer to char.
		 * It depends on the buffer's implementation, if the content
		 * is accessible after the buffer's release.
		 * The application should not use this pointer after the
		 * object was released.
		 */
		virtual const char *getDataAsCharPtr() const;
	};



	/**
	 * @brief A \ref DataBuffer implementation, which stores a simple data pointer.
	 * When the object is released, the buffer will be destroyed.
	 */
	class ExclusiveDataBuffer : public DataBuffer
	{
	private:
		ExclusiveDataBuffer() : data(NULL), size(0) {}

	public:
		/**
		 * @brief Creates a new \ref DataBuffer with a copy of \c data.
		 */
		static ExclusiveDataBuffer *createCopyOf(data_t data, size_t size);

		/**
		 * @brief Creates a new \ref DataBuffer with a copy of \c data.
		 */
		static ExclusiveDataBuffer *createCopyOf(const char *data, size_t size);

		/**
		 * @brief Creates a new \ref DataBuffer with a copy of \c data.
		 */
		static ExclusiveDataBuffer *createCopyOf(const std::string &str);

		/**
		 * @brief Creates a new \ref DataBuffer, which references to the given data.
		 * When the buffer is destroyed, the memory, which is referenced by \c data
		 * will be destroyed.
		 * To create a copy of an existing buffer use \ref ExclusiveDataBuffer::copyOf(data_t,size_t).
		 */
		ExclusiveDataBuffer(data_t data, size_t size);
		~ExclusiveDataBuffer();

		virtual data_t getData() const;
		virtual size_t getSize() const;

	private:
		data_t	data;
		size_t	size;
	};



	/**
	 * @brief A \ref DataBuffer implementation, which stores a simple data pointer.
	 * The buffer will NOT be destroyed, when the object is released.
	 */
	class SharedDataBuffer : public DataBuffer
	{
	private:
		SharedDataBuffer() : data(NULL), size(0) {}

	public:
		SharedDataBuffer(data_t data, size_t size);
		~SharedDataBuffer();

		virtual data_t getData() const;
		virtual size_t getSize() const;

	private:
		data_t	data;
		size_t	size;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_DATABUFFER_H__ */
