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
#include "connection_event_dispatcher.h"

#include <wiesel/engine.h>

using namespace wiesel;


void ConnectionEventDispatcher::dispatch(
			const ConnectionListeners&		listeners,
			ConnectionEvent					event,
			std::string						address,
			Connection*						connection
) {
	ConnectionEventDispatcher *dispatcher = new ConnectionEventDispatcher(
			listeners, event, address, connection
	);

	Engine::getInstance()->runOnMainThread(dispatcher);
}


ConnectionEventDispatcher::ConnectionEventDispatcher(
			const ConnectionListeners&		listeners,
			ConnectionEvent					event,
			std::string						address,
			Connection*						connection
) :
	listeners(listeners),
	event(event),
	address(address),
	connection(connection)
{
	return;
}

ConnectionEventDispatcher::~ConnectionEventDispatcher() {
	return;
}


void ConnectionEventDispatcher::run() {
	switch(event) {
		case ConnectionEvent_Connected: {
			for(ConnectionListeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
				(*it)->onConnected(address, connection);
			}

			break;
		}

		case ConnectionEvent_ConnectionFailed: {
			for(ConnectionListeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
				(*it)->onConnectionFailed(address);
			}

			break;
		}

		case ConnectionEvent_Disconnected: {
			for(ConnectionListeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
				(*it)->onDisconnected(address, connection);
			}

			break;
		}
	}

	return;
}

