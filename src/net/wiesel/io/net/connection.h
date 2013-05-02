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
#include <wiesel/wiesel-net.def>

#include <string>

namespace wiesel {

	class WIESEL_NET_EXPORT Connection : public virtual SharedObject
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
		typedef uint8_t data_t;

	public:
		virtual bool isConnected() const = 0;

		virtual void disconnect() = 0;

		virtual bool send(const data_t* data, size_t size) = 0;

		virtual int read(data_t *ptr, size_t size) = 0;

	private:

	};

}

#endif	// __WIESEL_IO_NET_CONNECTION_H__
