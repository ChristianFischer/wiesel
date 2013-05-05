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
#include "wiesel/util/log.h"
#include "wiesel/engine.h"


using namespace wiesel;



enum ConnectionEvent {
	ConnectionEvent_Connected,
	ConnectionEvent_ConnectionFailed,
	ConnectionEvent_Disconnected,
};


class ConnectionEventDispatcher : public IRunnable
{
public:
	ConnectionEventDispatcher(
			MessageDispatcher::Listeners	listeners,
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

	virtual ~ConnectionEventDispatcher() {
		return;
	}

public:
	virtual void run() {
		switch(event) {
			case ConnectionEvent_Connected: {
				for(MessageDispatcher::Listeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
					(*it)->onConnected(address, connection);
				}

				break;
			}

			case ConnectionEvent_ConnectionFailed: {
				for(MessageDispatcher::Listeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
					(*it)->onConnectionFailed(address);
				}

				break;
			}

			case ConnectionEvent_Disconnected: {
				for(MessageDispatcher::Listeners::iterator it=listeners.begin(); it!=listeners.end(); it++) {
					(*it)->onDisconnected(address, connection);
				}

				break;
			}
		}

		return;
	}

private:
	MessageDispatcher::Listeners	listeners;
	ConnectionEvent					event;
	std::string						address;
	ref<Connection>					connection;
};




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
			Engine::getInstance()->runOnMainThread(new ConnectionEventDispatcher(
					*(this->getListeners()),
					ConnectionEvent_Disconnected,
					this->address,
					connection
			));

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
				Engine::getInstance()->runOnMainThread(new ConnectionEventDispatcher(
						*(this->getListeners()),
						ConnectionEvent_Connected,
						this->address,
						connection
				));
			}
			else {
				Engine::getInstance()->runOnMainThread(new ConnectionEventDispatcher(
						*(this->getListeners()),
						ConnectionEvent_ConnectionFailed,
						this->address,
						NULL
				));
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
