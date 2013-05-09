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
#include "connection_datasource.h"
#include "connection.h"

#include <sstream>

using namespace wiesel;


ConnectionDataSource::ConnectionDataSource() {
	return;
}

ConnectionDataSource::ConnectionDataSource(const URI& uri) : uri(uri) {
	return;
}

ConnectionDataSource::~ConnectionDataSource() {
	return;
}


DataBuffer *ConnectionDataSource::getDataBuffer() {
	if (content == NULL) {
		ref<Connection> connection = Connection::createConnection(uri);
		if (connection) {
			std::stringstream ss;
			char c;

			while(connection->read((Connection::data_t*)&c, 1) > 0) {
				ss.put(c);
			}

			content = ExclusiveDataBuffer::createCopyOf(ss.str());
		}
	}

	return content;
}

void ConnectionDataSource::releaseDataBuffer() {
	content = NULL;
}
