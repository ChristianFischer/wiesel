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
#include "connection.h"
#include "connector.h"

#include "connection_event_dispatcher.h"
#include <wiesel/util/thread.h>

#include <wiesel/module_registry.h>

using namespace wiesel;


ConnectionListener::ConnectionListener() {
	return;
}

ConnectionListener::~ConnectionListener() {
	return;
}

void ConnectionListener::onConnected(const URI& uri, Connection* connection) {
	return;
}

void ConnectionListener::onConnectionFailed(const URI& uri) {
	return;
}

void ConnectionListener::onDisconnected(const URI& uri, Connection* connection) {
	return;
}




class CreateConnectionAsyncTask : public IRunnable
{
public:
	CreateConnectionAsyncTask(const URI& uri, ConnectionListener *listener) {
		this->uri		= uri;
		this->listener	= keep(listener);
	}

	virtual ~CreateConnectionAsyncTask() {
		safe_release(listener);
	}

	virtual void run() {
		ConnectionEventDispatcher::ConnectionListeners listeners;
		listeners.push_back(listener);

		Connection *connection = Connection::createConnection(uri);

		// attach the listener to a successfully created connection
		if (connection) {
			connection->addListener(listener);
		}

		ConnectionEventDispatcher::dispatch(
							listeners,
							connection
								? ConnectionEventDispatcher::ConnectionEvent_Connected
								: ConnectionEventDispatcher::ConnectionEvent_ConnectionFailed,
							uri,
							connection
		);

		return;
	}

private:
	URI						uri;
	ConnectionListener*		listener;
};





Connection::Connection() {
}

Connection::~Connection() {
}


Connection* Connection::createConnection(const std::string& address) {
	URI uri = URI::parse(address);
	if (uri.empty() == false) {
		return createConnection(uri);
	}

	return NULL;
}


Connection* Connection::createConnection(const URI& uri) {
	std::vector<ModuleLoader<IConnector>*> loaders = ModuleRegistry::getInstance()->findModules<IConnector>();
	for(std::vector<ModuleLoader<IConnector>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		ref<IConnector> connector = (*it)->create();
		if (connector == NULL) {
			continue;
		}

		Connection *connection = connector->createConnection(uri);
		if (connection) {
			return connection;
		}
	}

	return NULL;
}


void Connection::createConnectionAsync(const std::string& address, ConnectionListener* listener) {
	URI uri = URI::parse(address);
	if (uri.empty() == false) {
		return createConnectionAsync(uri, listener);
	}

	return;
}


void Connection::createConnectionAsync(const URI& uri, ConnectionListener* listener) {
	Thread *thread = new Thread(new CreateConnectionAsyncTask(uri, listener));
	thread->start();
	thread->detach();

	return;
}


void Connection::setCurrentURI(const URI& uri) {
	this->uri = uri;
}


void Connection::fireOnConnected() {
	for(Listeners::const_iterator it=listeners_begin(); it!=listeners_end(); it++) {
		(*it)->onConnected(getURI(), this);
	}

	return;
}

void Connection::fireOnConnectionFailed() {
	for(Listeners::const_iterator it=listeners_begin(); it!=listeners_end(); it++) {
		(*it)->onConnectionFailed(getURI());
	}

	return;
}

void Connection::fireOnDisconnected() {
	for(Listeners::const_iterator it=listeners_begin(); it!=listeners_end(); it++) {
		(*it)->onDisconnected(getURI(), this);
	}

	return;
}
