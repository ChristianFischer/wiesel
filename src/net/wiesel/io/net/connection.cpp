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

#include <wiesel/module_registry.h>

using namespace wiesel;


Connection::Connection() {
}

Connection::~Connection() {
}


Connection* Connection::createConnection(const std::string& address) {
	std::vector<ModuleLoader<IConnector>*> loaders = ModuleRegistry::getInstance()->findModules<IConnector>();
	for(std::vector<ModuleLoader<IConnector>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		ref<IConnector> connector = (*it)->create();
		if (connector == NULL) {
			continue;
		}

		Connection *connection = connector->createConnection(address);
		if (connection) {
			return connection;
		}
	}

	return NULL;
}
