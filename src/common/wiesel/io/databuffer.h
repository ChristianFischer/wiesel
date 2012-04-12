/*
 * databuffer.h
 *
 *  Created on: 11.04.2012
 *      Author: Baldur
 */

#ifndef __WIESEL_IO_DATABUFFER_H__
#define __WIESEL_IO_DATABUFFER_H__

#include "wiesel/util/shared_object.h"


namespace wiesel {

	/**
	 * @brief Wrapper object for any kind of data buffers.
	 */
	class DataBuffer : public SharedObject
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
