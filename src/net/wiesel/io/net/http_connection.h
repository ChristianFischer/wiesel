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
#ifndef __WIESEL_IO_NET_HTTP_CONNECTION_H__
#define __WIESEL_IO_NET_HTTP_CONNECTION_H__

#include "socket_connection.h"
#include "http_status_codes.h"

#include <map>

namespace wiesel {


	class HttpConnection;
	class HttpResponse;


	class WIESEL_NET_EXPORT HttpConnector : public IConnector
	{
	public:
		HttpConnector();
		virtual ~HttpConnector();

		virtual Connection* createConnection(const URI& uri);
	};



	/**
	 * @brief A connection class for sending requests to a HTTP server
	 * and receive data from it.
	 */
	class WIESEL_NET_EXPORT HttpConnection : public Connection
	{
	public:
		/**
		 * @brief The HTTP-Request method.
		 */
		enum Method {
			Method_GET,
			Method_POST,
		};

	public:
		HttpConnection();
		virtual ~HttpConnection();

		/**
		 * @brief Starts a new HTTP request.
		 * The client can send more data or call \ref finishRequest and
		 * wait for the server's response.
		 * @param uri		The URI to connect to.
		 * @param method	The request-method.
		 * @return A valid HttpConnection on success, or \c NULL on fail.
		 */
		static HttpConnection *createRequest(const URI& uri, Method method=Method_GET);

	// sending data
	public:
		/**
		 * @brief Send a single line as HTTP header for the current request.
		 */
		bool sendHeader(const std::string& name, const std::string& value);

		/**
		 * @brief Finish the request and tell the server to send the requested data.
		 * It's neccessary to call this function in order to receive data.
		 */
		bool finishRequest();

	// response
	public:
		/**
		 * @brief Blocks until a full response was received.
		 * When the request was successful, the client can start reading
		 * the requested data from the connection.
		 * @return A HTTP response code.
		 */
		HttpStatusCode waitForResponse();

		/**
		 * @brief Get the last response, if any.
		 */
		const HttpResponse* getLastResponse() const;

	// Connection
	public:
		virtual bool isConnected() const;
		virtual void disconnect();
		virtual bool send(const data_t* data, size_t size);
		virtual int  read(data_t *ptr, size_t size);

	private:
		ref<Connection>		internal_connection;
		ref<HttpResponse>	last_response;
	};



	/**
	 * @brief A class storing the result of a HTTP request.
	 */
	class WIESEL_NET_EXPORT HttpResponse : public virtual SharedObject
	{
	public:
		HttpResponse();
		virtual ~HttpResponse();

	public:
		/**
		 * @brief Read the response from a connection.
		 * @param connection	A connection to read from.
		 * @return	\c true on success. Even on success,
		 *			there's no guarantee to get valid date,
		 *			see the response status code for details.
		 */
		bool readFrom(Connection *connection);

	public:
		/**
		 * @brief Get the HTTP Status Code returned to this response.
		 */
		inline HttpStatusCode getStatusCode() const {
			return status_code;
		}

		/**
		 * @brief Get a specific attribute from the HTTP response.
		 */
		std::string get(const std::string& attrib) const;

	private:
		typedef std::map<std::string, std::string>	ParameterMap;

		HttpStatusCode		status_code;
		ParameterMap		parameters;
	};

}

#endif // __WIESEL_IO_NET_HTTP_CONNECTION_H__

