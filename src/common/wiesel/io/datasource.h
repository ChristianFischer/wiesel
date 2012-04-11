/*
 * datasource.h
 *
 *  Created on: 11.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_IO_DATASOURCE_H__
#define __WIESEL_IO_DATASOURCE_H__

#include "databuffer.h"
#include "wiesel/util/shared_object.h"


namespace wiesel {

	/**
	 * @brief Wrapper object for any kind of data source, like files or buffers.
	 */
	class DataSource : public SharedObject
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
	};



	/**
	 * @brief A DataSource which provides data of a \ref DataBuffer object.
	 */
	class BufferDataSource : public DataSource
	{
	private:
		BufferDataSource() : buffer(NULL) {}

	public:
		/**
		 * @brief Creates a new \ref BufferDataSource with existing data.
		 */
		BufferDataSource(DataBuffer::data_t data, size_t size);

		/**
		 * @brief Creates a new \ref BufferDataSource with an existing \ref DataBuffer.
		 */
		BufferDataSource(DataBuffer *buffer);

		virtual ~BufferDataSource();

		virtual DataBuffer *getDataBuffer();

	private:
		DataBuffer* buffer;
	};

} /* namespace wiesel */
#endif /* __WIESEL_IO_DATASOURCE_H__ */
