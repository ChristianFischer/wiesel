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
#ifndef __WIESEL_IO_URI_H__
#define	__WIESEL_IO_URI_H__

#include <wiesel/wiesel-base.def>
#include <list>
#include <stdint.h>
#include <string>
#include <ostream>

namespace wiesel {


	class WIESEL_BASE_EXPORT URI
	{
	public:
		URI();
		URI(const URI& orig);
		~URI();

		static URI parse(const std::string& uri);

	public:
		/// A single element within the URI's query string.
		struct QueryElement {
			std::string	arg;
			std::string	value;
		};

		/// type alias for the complete query list
		typedef std::list<QueryElement> Query;

	// getter / setter
	public:
		/// check if this URI is empty.
		bool empty() const;

		/// set the URI's scheme.
		void setScheme(const std::string& scheme);

		/// get the URI's scheme.
		const std::string& getScheme() const;

		/// set the URI's authority.
		void setAuthority(const std::string& authority);

		/// get the URI's authority.
		std::string getAuthority() const;

		/// set the userinfo of this URI
		void setUserInfo(const std::string& userinfo);

		/// get the user part of this URI including username and password
		std::string getUserInfo() const;

		/// set the URI's username component.
		void setUserName(const std::string& user);

		/// get the URI's username component.
		const std::string& getUserName() const;

		/// set the URI's password component.
		void setPassword(const std::string& password);

		/// get the URI's password component.
		const std::string& getPassword() const;

		/// set the URI's host.
		void setHost(const std::string& host);

		/// get the URI's host.
		const std::string& getHost() const;

		/// set the URI's port.
		void setPort(uint32_t port);

		/// get the URI's port or 0, when no specific port was defined.
		uint32_t getPort() const;

		/// returns \c true, when this URI has a port specified.
		bool hasPort() const;

		/// set the URI'S path.
		void setPath(const std::string& path);

		/// get the URI's path.
		const std::string& getPath() const;

		/// set the URI's query.
		void setQueryString(const std::string& query);

		/// get the URI's qurty as string.
		std::string getQueryString() const;

		/// get the URI's query elements.
		const Query& getQuery() const;

		/// add a new argument to the URI's query or replace an existing one
		void setQueryArg(const std::string& arg, const std::string& value);

		/// get a single query argument from this URI.
		std::string getQueryArg(const std::string& arg) const;

		/// set the URI's fragment.
		void setFragment(const std::string& fragment);

		/// get the URI's fragment.
		const std::string& getFragment() const;

		/// get this URI as string.
		std::string toString() const;

	private:
		std::string		scheme;
		std::string		username;
		std::string		password;
		std::string		host;
		uint32_t		port;
		std::string		path;
		Query			query;
		std::string		fragment;
	};



	WIESEL_BASE_EXPORT std::ostream& operator <<(std::ostream &o, const URI& uri);


}

#endif // __WIESEL_IO_URI_H__

