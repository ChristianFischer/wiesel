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

#include <wiesel/io/uri.h>
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
		 * @param uri			The address of the connection.
		 * @param connection	The created connection.
		 */
		virtual void onConnected(const URI& uri, Connection* connection);

		/**
		 * @brief The connection couldn't be created successfully.
		 * @param uri			The address of the connection.
		 */
		virtual void onConnectionFailed(const URI& uri);

		/**
		 * @brief The connection was closed.
		 * @param uri			The address of the connection.
		 * @param connection	The connection which was closed.
		 */
		virtual void onDisconnected(const URI& uri, Connection* connection);
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
		 * @param address	The address to connect to.
		 * @return A new \ref Connection object or \c NULL, when the connection was not successful.
		 */
		static Connection* createConnection(const std::string& address);

		/**
		 * @brief Creates a new connection to a specific address.
		 * @param uri		The address to connect to.
		 * @return A new \ref Connection object or \c NULL, when the connection was not successful.
		 */
		static Connection* createConnection(const URI& uri);

		/**
		 * @brief Tries to connect to the given address within a seperate thread.
		 * @param address	The address to connect to.
		 * @param listener	The listener which will receive the created connection on success
		 *					or an onConnectionFailed event, when no connection could be established.
		 *					When successful, the listener will be attached to the created connection.
		 */
		static void createConnectionAsync(const std::string& address, ConnectionListener *listener);

		/**
		 * @brief Tries to connect to the given address within a seperate thread.
		 * @param uri		The address to connect to.
		 * @param listener	The listener which will receive the created connection on success
		 *					or an onConnectionFailed event, when no connection could be established.
		 *					When successful, the listener will be attached to the created connection.
		 */
		static void createConnectionAsync(const URI& uri, ConnectionListener *listener);

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
		void setCurrentURI(const URI& uri);

	public:
		/**
		 * @brief Get the address of this connection.
		 */
		const URI& getURI() const {
			return uri;
		}

	protected:
		void fireOnConnected();
		void fireOnConnectionFailed();
		void fireOnDisconnected();

	private:
		URI uri;
	};

}

#endif	// __WIESEL_IO_NET_CONNECTION_H__
