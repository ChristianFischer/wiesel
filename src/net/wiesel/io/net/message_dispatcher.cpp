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
#include "message_dispatcher.h"
#include "connection_event_dispatcher.h"
#include "wiesel/util/log.h"
#include "wiesel/engine.h"


using namespace wiesel;


MessageDispatcher::IMessage::IMessage() {
	return;
}

MessageDispatcher::IMessage::~IMessage() {
	return;
}



MessageDispatcher::MessageDispatcher() {
	this->connection	= NULL;
	this->thread		= NULL;
	this->stopped		= false;
	this->flags			= 0;
	return;
}

MessageDispatcher::~MessageDispatcher() {
	safe_release(connection);
	safe_release(thread);
}


void MessageDispatcher::addFlag(DispatchFlag flag) {
	this->flags |= flag;
}

void MessageDispatcher::removeFlag(DispatchFlag flag) {
	this->flags &= ~flag;
}

bool MessageDispatcher::hasFlag(DispatchFlag flag) const {
	return ((flags & flag) != 0);
}


void MessageDispatcher::start(const std::string& address) {
	if (thread == NULL) {
		this->address = address;
		this->stopped = false;

		thread = keep(new Thread(this));
		thread->start();
		thread->detach();
	}

	return;
}


void MessageDispatcher::stop() {
	this->stopped = true;
	return;
}


void MessageDispatcher::lock() {
	if (thread) {
		thread->lock();
	}

	return;
}


void MessageDispatcher::unlock() {
	if (thread) {
		thread->unlock();
	}

	return;
}


void MessageDispatcher::run() {
	Thread *current_thread = keep(this->thread);

	do {
		// delete connection, when interrupted
		if (connection && (connection->isConnected() == false)) {
			ConnectionEventDispatcher::dispatch(
					*(this->getListeners()),
					ConnectionEventDispatcher::ConnectionEvent_Disconnected,
					this->address,
					connection
			);

			// release the object (we're creating a new one)
			safe_release(connection);

			// stop here, when no reconnect-flag is set
			if (!hasFlag(AutoReconnect)) {
				break;
			}
		}

		// try to reconnect, if neccessary
		if (connection == NULL) {
			connection = keep(Connection::createConnection(this->address));

			// notify, when the connection was successful
			if (connection) {
				ConnectionEventDispatcher::dispatch(
						*(this->getListeners()),
						ConnectionEventDispatcher::ConnectionEvent_Connected,
						this->address,
						connection
				);
			}
			else {
				ConnectionEventDispatcher::dispatch(
						*(this->getListeners()),
						ConnectionEventDispatcher::ConnectionEvent_ConnectionFailed,
						this->address,
						NULL
				);
			}
		}

		// when the connection is OK, try to fetch messages
		if (connection && connection->isConnected()) {
			fetchMessage(connection);
		}
		else {
			// otherwise wait a moment
			Thread::sleep(1000);
		}
	}
	while(!stopped);

	// done - cleanup
	current_thread->lock();

	if (this->thread) {
		safe_release(this->thread);
	}

	if (connection) {
		connection->disconnect();
		safe_release(connection);
	}

	if (current_thread) {
		current_thread->unlock();
		release(current_thread);
	}

	return;
}
