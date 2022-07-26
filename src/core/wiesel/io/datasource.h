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
#ifndef __WIESEL_IO_DATASOURCE_H__
#define __WIESEL_IO_DATASOURCE_H__

#include <wiesel/wiesel-core.def>

#include "databuffer.h"
#include "wiesel/util/shared_object.h"


namespace wiesel {

	/**
	 * @brief Wrapper object for any kind of data source, like files or buffers.
	 */
	class WIESEL_CORE_EXPORT DataSource : public virtual SharedObject
	{
	protected:
		DataSource();

	public:
		virtual ~DataSource();

		/**
		 * @brief Get the \ref DataBuffer of this data source.
		 * This is the most universal method to get the data.
		 */
		virtual DataBuffer *getDataBuffer() = 0;
		
		/**
		 * @brief Release the data buffer, when it is no longer required.
		 * Since the DataBuffer returned by \ref getDataBuffer() may be cached
		 * to be accessed multiple times, it is recommended to release 
		 * the buffer, when no longer required.
		 */
		virtual void releaseDataBuffer() = 0;
	};



	/**
	 * @brief A DataSource which provides data of a \ref DataBuffer object.
	 */
	class WIESEL_CORE_EXPORT BufferDataSource : public DataSource
	{
	private:
		BufferDataSource() : buffer(NULL) {}

	public:
		/**
		 * @brief Creates a new \ref BufferDataSource with existing data.
		 */
		BufferDataSource(DataBuffer::mutable_data_t data, size_t size);

		/**
		 * @brief Creates a new \ref BufferDataSource with an existing \ref DataBuffer.
		 */
		BufferDataSource(DataBuffer *buffer);

		virtual ~BufferDataSource();

		virtual DataBuffer *getDataBuffer();
		
		virtual void releaseDataBuffer();

	private:
		DataBuffer* buffer;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_DATASOURCE_H__ */
