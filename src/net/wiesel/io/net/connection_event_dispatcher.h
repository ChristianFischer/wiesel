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
#ifndef __WIESEL_IO_NET_CONNECTION_EVENT_DISPATCHER_H__
#define	__WIESEL_IO_NET_CONNECTION_EVENT_DISPATCHER_H__

#include "connection.h"
#include "wiesel/util/thread.h"

#include <list>

namespace wiesel {

	/**
	 * @brief Utility class for dispatching connection events to the mainthread.
	 */
	class ConnectionEventDispatcher : public IRunnable
	{
	public:
		typedef std::list<ConnectionListener*> ConnectionListeners;

		enum ConnectionEvent {
			ConnectionEvent_Connected,
			ConnectionEvent_ConnectionFailed,
			ConnectionEvent_Disconnected,
		};


		static void dispatch(
				const ConnectionListeners&	listeners,
				ConnectionEvent				event,
				const URI&					uri,
				Connection*					connection
		);

		ConnectionEventDispatcher(
				const ConnectionListeners&	listeners,
				ConnectionEvent				event,
				const URI&					uri,
				Connection*					connection
		);

		virtual ~ConnectionEventDispatcher();

	public:
		virtual void run();

	private:
		ConnectionListeners		listeners;
		ConnectionEvent			event;
		URI						uri;
		ref<Connection>			connection;
	};


}

#endif // __WIESEL_IO_NET_CONNECTION_EVENT_DISPATCHER_H__

