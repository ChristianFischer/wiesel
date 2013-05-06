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
#include "socket_connection.h"

#include <wiesel/util/log.h>

#include <stdlib.h>
#include <sstream>


#include <wiesel/platform_config.h>

#if WIESEL_PLATFORM_UNIX
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if WIESEL_PLATFORM_WINDOWS
#include <winsock2.h>
#endif


using namespace wiesel;


SocketConnector::SocketConnector() {
	#if WIESEL_PLATFORM_WINDOWS
		WSADATA w;
		int result;

		if ((result = WSAStartup(MAKEWORD(2, 2), &w)) != 0) {
			Log::err << "Failed to initialize WinSock! Error #" << result << std::endl;
		}
	#endif

	return;
}

SocketConnector::~SocketConnector() {
	return;
}


Connection* SocketConnector::createConnection(const std::string& address) {
	SocketConnection *connection = NULL;

	std::string::size_type seperator = address.rfind(':');
	if (seperator != std::string::npos) {
		connection = new SocketConnection();
		bool success = connection->connect(
									address.substr(0, seperator),
									atoi(address.c_str() + seperator + 1)
		);

		if (!success) {
			delete connection;
			connection = NULL;
		}
	}

	return connection;
}




SocketConnection::SocketConnection() {
	this->socket_handle		= -1;
}

SocketConnection::~SocketConnection() {
	assert(isConnected() == false);
	disconnect();
}

bool SocketConnection::connect(const std::string& host, uint32_t port) {
	bool connected = false;
	
	// check if there's already an existing connection
	assert(isConnected() == false);
	if (isConnected()) {
		return false;
	}

	// set the current address (the address should be updated, even if no connection could be established).
	{
		std::stringstream ss;
		ss << host;
		ss << ':';
		ss << port;

		setCurrentAddress(ss.str());
	}

	socket_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_handle == -1) {
		return false;
	}

	hostent* he = gethostbyname(host.c_str());
	if (he == NULL) {
		releaseSocket();
		fireOnConnectionFailed();

		return false;
	}

	sockaddr_in addr;
	addr.sin_family		= he->h_addrtype;
	addr.sin_port		= htons(static_cast<uint16_t>(port));

	for(char **p=he->h_addr_list; *p; ++p) {
		addr.sin_addr	= *reinterpret_cast<in_addr*>(*p);

		if (addr.sin_addr.s_addr == INADDR_NONE) {
			continue;
		}

		if (addr.sin_addr.s_addr == INADDR_ANY) {
			continue;
		}

		int result = ::connect(socket_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
		if (result == -1) {
			continue;
		}

		// connection successful
		connected = true;

		break;
	}

	// release the socket, when not connected
	if (!connected) {
		releaseSocket();
	}

	// fire notification
	if (connected) {
		fireOnConnected();
	}
	else {
		fireOnConnectionFailed();
	}

	return connected;
}


bool SocketConnection::isConnected() const {
	if (socket_handle != -1) {
		return true;
	}

	return false;
}


void SocketConnection::disconnect() {
	if (socket_handle != -1) {
		releaseSocket();
		fireOnDisconnected();
	}

	return;
}


bool SocketConnection::send(const data_t *data, size_t size) {
	size_t bytes_sent	= 0;

	do {
		int result = ::send(socket_handle, (const char*)(data + bytes_sent), size - bytes_sent, 0);

		if (result < 0) {
			return false;
		}

		bytes_sent += result;
	}
	while(bytes_sent < size);

	return true;
}


int SocketConnection::read(data_t *ptr, size_t size) {
	int result = ::recv(socket_handle, (char*)ptr, size, 0);
	if (result <= 0) {
		disconnect();
	}

	return result;
}


void SocketConnection::releaseSocket() {
	if (socket_handle != -1) {
		#if WIESEL_PLATFORM_UNIX
			close(socket_handle);
		#endif

		#if WIESEL_PLATFORM_WINDOWS
			closesocket(socket_handle);
		#endif

		socket_handle = -1;
	}

	return;
}

