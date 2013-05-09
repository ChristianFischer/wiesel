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
#include "http_connection.h"

#include <cstdlib>
#include <sstream>
#include <map>

#include "wiesel/util/log.h"

using namespace wiesel;


HttpConnector::HttpConnector() {
	return;
}

HttpConnector::~HttpConnector() {
	return;
}


Connection* HttpConnector::createConnection(const URI& uri) {
	if (
			(uri.getScheme() == "http")
		||	(uri.getScheme() == "https")
		||	(uri.getScheme() == "" && uri.getHost().substr(4) == "www.")
	) {
		HttpConnection *connection = HttpConnection::createRequest(uri);
		if (connection) {
			connection->finishRequest();

			HttpStatusCode response = connection->waitForResponse();
			if (response >= 200 && response <= 299) {
				return connection;
			}
			else {
				delete connection;
			}
		}
	}

	return NULL;
}



HttpConnection::HttpConnection() {
}

HttpConnection::~HttpConnection() {
}


HttpConnection* HttpConnection::createRequest(const URI& uri, Method method) {
	HttpConnection *connection = new HttpConnection();
	connection->setCurrentURI(uri);

	URI socket_uri;
	socket_uri.setHost(uri.getHost());
	socket_uri.setPort(uri.hasPort() ? uri.getPort() : 80);

	connection->internal_connection = Connection::createConnection(socket_uri);
	if (connection->isConnected() == false) {
		delete connection;
		return NULL;
	}

	const std::string HTTP_ENDL("\r\n");
	std::stringstream ss;

	switch(method) {
		case Method_GET: {
			ss << "GET";
			break;
		}

		case Method_POST: {
			ss << "POST";
			break;
		}
	}

	ss << ' ';
	ss << (uri.getPath().empty() ? "/" : uri.getPath());

	const std::string query = uri.getQueryString();
	if (query.empty() == false) {
		ss << '?';
		ss << query;
	}

	ss << " HTTP/1.1" << HTTP_ENDL;

	std::string header_request = ss.str();
	connection->send((const data_t*)(header_request.c_str()), header_request.length());

	connection->sendHeader("Host", connection->getURI().getAuthority());

	return connection;
}


bool HttpConnection::sendHeader(const std::string& name, const std::string& value) {
	std::stringstream ss;
	ss << name;
	ss << ": ";
	ss << value;
	ss << "\r\n";

	std::string line = ss.str();

	return send((const data_t*)(line.c_str()), line.length());
}


bool HttpConnection::finishRequest() {
	return send((const data_t*)("\r\n"), 2);
}


HttpStatusCode HttpConnection::waitForResponse() {
	// clear last response
	last_response = NULL;

	HttpResponse *response = new HttpResponse();
	bool success = response->readFrom(this);

	if (success) {
		last_response = response;
		return response->getStatusCode();
	}

	if (!success) {
		delete response;
	}

	return HTTP_NotFound;
}


const HttpResponse* HttpConnection::getLastResponse() const {
	return last_response;
}




bool HttpConnection::isConnected() const {
	return (internal_connection!=NULL) && (internal_connection->isConnected());
}


void HttpConnection::disconnect() {
	if (internal_connection) {
		internal_connection->disconnect();
	}
}


bool HttpConnection::send(const data_t* data, size_t size) {
	if (internal_connection) {
		bool result = internal_connection->send(data, size);

		if (result == false) {
			disconnect();
		}

		return result;
	}

	return false;
}


int HttpConnection::read(data_t *ptr, size_t size) {
	if (internal_connection) {
		int result = internal_connection->read(ptr, size);

		if (result <= 0) {
			disconnect();
		}

		return result;
	}

	return 0;
}





HttpResponse::HttpResponse() {
	this->status_code = HTTP_NotFound;
}

HttpResponse::~HttpResponse() {
	return;
}


bool HttpResponse::readFrom(Connection* connection) {
	std::stringstream ss;

	do {
		char c;

		do {
			int result = connection->read((Connection::data_t*)&c, 1);

			// connection interrupted
			if (result <= 0 || !connection->isConnected()) {
				status_code = HTTP_ServiceUnavailable;
				return false;
			}

			if (c == '\r') {
				continue;
			}

			if (c == '\n') {
				break;
			}

			ss.put(c);
		}
		while(true);

		std::string line = ss.str();
		ss.str("");

		// done
		if (line.empty()) {
			break;
		}

		if (line.substr(0, 5) == "HTTP/") {
			if (
					line.substr(5, 3) == "1.0"
				||	line.substr(5, 3) == "1.1"
			) {
				int num = std::atoi(line.substr(9, 3).c_str());
				if (num) {
					status_code = static_cast<HttpStatusCode>(num);
				}
			}

			continue;
		}

		std::string::size_type seperator = line.find(": ");
		if (seperator != std::string::npos) {
			parameters.insert(std::pair<std::string,std::string>(
							line.substr(0, seperator),
							line.substr(seperator + 2)
			));
		}
	}
	while(true);

	return true;
}


std::string HttpResponse::get(const std::string& attrib) const {
	ParameterMap::const_iterator it = parameters.find(attrib);
	if (it != parameters.end()) {
		return it->second;
	}

	return std::string();
}

