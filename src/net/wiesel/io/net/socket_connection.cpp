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
	int result;

	socket_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_handle == -1) {
		return false;
	}

	sockaddr_in addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(static_cast<uint16_t>(port));
	addr.sin_addr.s_addr	= inet_addr(host.c_str());

	if (addr.sin_addr.s_addr == INADDR_NONE) {
		return false;
	}

	if (addr.sin_addr.s_addr == INADDR_ANY) {
		return false;
	}

	result = ::connect(socket_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (result == -1) {
		disconnect();
		return false;
	}

	return true;
}


bool SocketConnection::isConnected() const {
	if (socket_handle != -1) {
		return true;
	}

	return false;
}


void SocketConnection::disconnect() {
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
	int result = ::recv(socket_handle, (char*)ptr, 1, 0);
	return result;
}


