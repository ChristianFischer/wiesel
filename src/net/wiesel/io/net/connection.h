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
#ifndef __WIESEL_IO_NET_CONNECTION_H__
#define	__WIESEL_IO_NET_CONNECTION_H__

#include <wiesel/util/shared_object.h>
#include <wiesel/util/listener_support.h>
#include <wiesel/wiesel-net.def>

#include <stdint.h>
#include <string>

namespace wiesel {

	class Connection;


	/**
	 * @brief A listener class receiving connection events.
	 */
	class WIESEL_NET_EXPORT ConnectionListener : public virtual SharedObject
	{
	public:
		ConnectionListener();
		virtual ~ConnectionListener();

	public:
		/**
		 * @brief Notification when the connection was successfully created.
		 * @param address		The address of the connection.
		 * @param connection	The created connection.
		 */
		virtual void onConnected(const std::string& address, Connection* connection);

		/**
		 * @brief The connection couldn't be created successfully.
		 * @param address		The address of the connection.
		 */
		virtual void onConnectionFailed(const std::string& address);

		/**
		 * @brief The connection was closed.
		 * @param address		The address of the connection.
		 * @param connection	The connection which was closed.
		 */
		virtual void onDisconnected(const std::string& address, Connection* connection);
	};



	/**
	 * @brief Baseclass for all network connections.
	 */
	class WIESEL_NET_EXPORT Connection :
			public virtual SharedObject,
			public ListenerSupport<ConnectionListener>
	{
	protected:
		Connection();

	public:
		virtual ~Connection();

		/**
		 * @brief Creates a new connection to a specific address.
		 * For TCP Connections, this address should be in the format host:port
		 * @return A new \ref Connection object or \c NULL, when the connection was not successful.
		 */
		static Connection* createConnection(const std::string& address);

	public:
		/// alias type for data sent via network.
		typedef uint8_t data_t;

	public:
		/**
		 * @brief Checks whether this connection is connected or not.
		 */
		virtual bool isConnected() const = 0;

		/**
		 * @brief Disconnect this connection.
		 */
		virtual void disconnect() = 0;

		/**
		 * @brief Send the given data over network.
		 * This function will block until all data was sent.
		 * @param data	Pointer to the data to be sent.
		 * @param size	The size of the data.
		 * @return \c true when all data was sent successfully.
		 */
		virtual bool send(const data_t* data, size_t size) = 0;

		/**
		 * @brief Read data from the network connection.
		 * @param ptr	Pointer to memory where to store the result.
		 * @param size	Size of the data to be read.
		 * @return \c The number of bytes which were read successfully.
		 */
		virtual int read(data_t *ptr, size_t size) = 0;

	protected:
		/**
		 * @brief Set the current address of this connection.
		 * This should be used by subclasses only, when trying to establish a connection.
		 */
		void setCurrentAddress(const std::string& address);

	public:
		/**
		 * @brief Get the address of this connection.
		 */
		const std::string& getAddress() const {
			return address;
		}

	protected:
		void fireOnConnected();
		void fireOnConnectionFailed();
		void fireOnDisconnected();

	private:
		std::string address;
	};

}

#endif	// __WIESEL_IO_NET_CONNECTION_H__
