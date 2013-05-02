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
#ifndef __WIESEL_IO_NET_CONNECTOR_H__
#define	__WIESEL_IO_NET_CONNECTOR_H__

#include <wiesel/module.h>
#include <wiesel/wiesel-net.def>

#include <string>

namespace wiesel {

	class Connection;

	class WIESEL_NET_EXPORT IConnector : public Module
	{
	protected:
		IConnector();

	public:
		virtual ~IConnector();

		virtual Connection* createConnection(const std::string& address) = 0;
	};

}

#endif	// __WIESEL_IO_NET_CONNECTOR_H__
