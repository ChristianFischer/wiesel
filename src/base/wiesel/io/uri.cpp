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
#include "uri.h"

#include <cstdlib>
#include <sstream>

using namespace wiesel;


URI::URI() : port(0) {
}

URI::URI(const URI& other) :
		scheme		(other.scheme),
		username	(other.username),
		password	(other.password),
		host		(other.host),
		port		(other.port),
		path		(other.path),
		query		(other.query),
		fragment	(other.fragment)
{
	return;
}

URI::~URI() {
}



URI URI::parse(const std::string& struri) {
	URI uri;

	std::string::size_type scheme_seperator = struri.find("://");
	if (scheme_seperator != std::string::npos) {
		uri.scheme = struri.substr(0, scheme_seperator);
	}
	
	std::string::size_type authority_begin =
							scheme_seperator == std::string::npos
						?	0
						:	scheme_seperator + 3
	;

	// get authority
	std::string::size_type authority_length = 0;
	{
		do {
			std::string::size_type next = authority_begin + authority_length;

			if (
					next >= struri.length()
				||	struri[next] == '/'
				||	struri[next] == '?'
				||	struri[next] == '#'
			) {
				break;
			}

			++authority_length;
		}
		while(true);

		uri.setAuthority(struri.substr(authority_begin, authority_length));
	}

	// get path
	std::string::size_type path_begin  = authority_begin + authority_length;
	std::string::size_type path_length = 0;
	{
		do {
			std::string::size_type next = path_begin + path_length;

			if (
					next >= struri.length()
				||	struri[next] == '?'
				||	struri[next] == '#'
			) {
				break;
			}

			++path_length;
		}
		while(true);

		uri.setPath(struri.substr(path_begin, path_length));
	}

	// get query
	std::string::size_type query_begin  = path_begin + path_length;
	std::string::size_type query_length = 0;
	{
		if (query_begin < struri.length() && struri[query_begin] == '?') {
			++query_begin;
		}

		do {
			std::string::size_type next = query_begin + query_length;

			if (
					next >= struri.length()
				||	struri[next] == '#'
			) {
				break;
			}

			++query_length;
		}
		while(true);

		uri.setQueryString(struri.substr(query_begin, query_length));
	}

	// get fragment
	std::string::size_type fragment_begin = query_begin + query_length;
	if (fragment_begin < struri.length() && struri[fragment_begin] == '#') {
		++fragment_begin;
	}

	uri.setFragment(struri.substr(fragment_begin));

	return uri;
}


bool URI::empty() const {
	return
			scheme.empty()
		&&	host.empty()
		&&	path.empty()
	;
}


void URI::setScheme(const std::string& scheme) {
	this->scheme = scheme;
}

const std::string& URI::getScheme() const {
	return scheme;
}


void URI::setAuthority(const std::string& authority) {
	std::string::size_type userinfo_seperator = authority.find('@');
	std::string::size_type begin_host = 0;

	if (userinfo_seperator != std::string::npos) {
		setUserInfo(authority.substr(0, userinfo_seperator));
		begin_host = userinfo_seperator + 1;
	}

	std::string::size_type port_seperator;
	port_seperator = authority.rfind(":", std::string::npos);
	if (port_seperator <= begin_host) {
		port_seperator  = std::string::npos;
	}

	if (port_seperator == std::string::npos) {
		setHost(authority.substr(begin_host));
		setPort(0u);
	}
	else {
		setHost(authority.substr(begin_host, port_seperator - begin_host));
		setPort(std::atoi(authority.substr(port_seperator + 1).c_str()));
	}

	return;
}


std::string URI::getAuthority() const {
	std::stringstream ss;

	if (!host.empty()) {
		if (!username.empty()) {
			ss << username;

			if (!password.empty()) {
				ss << ':' << password;
			}

			ss << '@';
		}

		ss << host;

		if (hasPort()) {
			ss << ':';
			ss << port;
		}
	}

	return ss.str();
}


void URI::setUserInfo(const std::string& userinfo) {
	std::string::size_type seperator = userinfo.find(':');
	if (seperator == std::string::npos) {
		setUserName(userinfo);
		setPassword(std::string());
	}
	else {
		setUserName(userinfo.substr(0, seperator));
		setPassword(userinfo.substr(seperator + 1));
	}
}


std::string URI::getUserInfo() const {
	if (!username.empty()) {
		std::stringstream ss;
		ss << username;

		if (!password.empty()) {
			ss << ':';
			ss << password;
		}

		return ss.str();
	}

	return std::string();
}


void URI::setUserName(const std::string& user) {
	this->username = user;
}

const std::string& URI::getUserName() const {
	return username;
}


void URI::setPassword(const std::string& password) {
	this->password = password;
}

const std::string& URI::getPassword() const {
	return password;
}


void URI::setHost(const std::string& host) {
	this->host = host;
}

const std::string& URI::getHost() const {
	return host;
}


void URI::setPort(uint32_t port) {
	this->port = port;
}

uint32_t URI::getPort() const {
	return port;
}

bool URI::hasPort() const {
	return port != 0;
}


void URI::setPath(const std::string& path) {
	this->path = path;
}

const std::string& URI::getPath() const {
	return path;
}


void URI::setQueryString(const std::string& strquery) {
	this->query.clear();

	std::string::size_type pos = 0;

	while(pos < strquery.length()) {
		std::string::size_type next = strquery.find('&', pos);
		if (next == std::string::npos) {
			next = strquery.length();
		}

		std::string::size_type seperator = strquery.find("=", pos);
		if (seperator > next) {
			seperator = std::string::npos;
		}

		if (seperator == std::string::npos) {
			setQueryArg(strquery.substr(pos, (next - pos)), "");
		}
		else {
			setQueryArg(
						strquery.substr(pos, (seperator - pos)),
						strquery.substr(seperator + 1, (next - seperator - 1))
			);
		}

		pos = next + 1;
	}

	return;
}


std::string URI::getQueryString() const {
	std::stringstream ss;

	for(Query::const_iterator it=query.begin(); it!=query.end(); it++) {
		if (ss.tellp()) {
			ss << '&';
		}

		ss << it->arg;
		ss << '=';
		ss << it->value;
	}

	return ss.str();
}


const URI::Query& URI::getQuery() const {
	return query;
}


void URI::setQueryArg(const std::string& arg, const std::string& value) {
	for(Query::iterator it=query.begin(); it!=query.end(); it++) {
		if (it->arg == arg) {
			it->value = value;
			return;
		}
	}

	// no existing element found - add a new one
	QueryElement element;
	element.arg   = arg;
	element.value = value;

	query.push_back(element);

	return;
}


std::string URI::getQueryArg(const std::string& arg) const {
	for(Query::const_reverse_iterator it=query.rbegin(); it!=query.rend(); it++) {
		if (it->arg == arg) {
			return it->value;
		}
	}

	return std::string();
}


void URI::setFragment(const std::string& fragment) {
	this->fragment = fragment;
}

const std::string& URI::getFragment() const {
	return fragment;
}


std::string URI::toString() const {
	std::stringstream ss;

	if (!scheme.empty()) {
		ss << scheme;
		ss << "://";
	}

	ss << getAuthority();
	ss << getPath();

	std::string _query = getQueryString();
	if (!_query.empty()) {
		ss << '?';
		ss << _query;
	}

	if (!fragment.empty()) {
		ss << '#';
		ss << fragment;
	}

	return ss.str();
}



std::ostream& wiesel::operator <<(std::ostream &o, const URI& uri) {
	o << uri.toString();
	return o;
}
