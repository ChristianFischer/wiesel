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
#include "log.h"

#include <sstream>
#include <string>
#include <stdio.h>

using namespace wiesel;
using namespace std;


// set the highest log level per default.
LogLevel Log::current_log_level = LogLevel_Debug;




/// implements the current platforms logging mechanism.
int _logmsg_impl(LogLevel level, const char *tag, const char *message);



/*
int logmsg(LogLevel level, const char *message, ...) {
	if (Log::isLogged(level, NULL)) {
		char *buffer;

		va_list args;
		va_start(args, message);
		vasprintf(&buffer, message, args);
		va_end(args);

		int ret = _logmsg_impl(level, "wiesel", buffer);
		free(buffer);

		return ret;
	}

	return 0;
}
*/


int wiesel::logmsg(LogLevel level, const char *tag, const char *message, ...) {
	if (Log::isLogged(level, tag)) {
		char *buffer;

		va_list args;
		va_start(args, message);
		vasprintf(&buffer, message, args);
		va_end(args);

		int ret = _logmsg_impl(level, tag?tag:WIESEL_LOG_TAG, buffer);
		free(buffer);

		return ret;
	}

	return 0;
}





/**
 * @brief The streambuffer class which is used by \ref Log to write lines into the current log.
 */
class wlog_streambuffer
:	public std::basic_streambuf<char, std::char_traits<char> >
{
typedef std::char_traits<char> _Tr;

public:
	wlog_streambuffer(LogLevel level) {
		this->level = level;
		return;
	}

protected:
	virtual int overflow(int c = _Tr::eof()) {
		if (c == _Tr::eof() || c == '\n') {
			writeCurrentLine();
			return _Tr::not_eof(c);
		}

		// buffer current character.
		ss.put(c);

		return c;
	}

private:
	int writeCurrentLine() {
		int ret = logmsg(level, tag.empty() ? NULL : tag.c_str(), ss.str().c_str());
		ss.str("");
		ss.clear();
		return ret;
	}

private:
	LogLevel		level;
	string			tag;
	stringstream	ss;
};




Log::Log(basic_streambuf<char, std::char_traits<char> > *buffer)
: basic_ostream<char, std::char_traits<char> >(buffer)
{
	clear();
	return;
}

Log::~Log() {
	return;
}


void Log::setLevel(LogLevel level) {
	Log::current_log_level = level;
}


bool Log::isLogged(LogLevel level, const char *tag) {
	if (level <= current_log_level) {
		return true;
	}

	return false;
}




// implement stream buffers for each log level
wlog_streambuffer	_buffer_error (LogLevel_Error);
wlog_streambuffer	_buffer_warn  (LogLevel_Warning);
wlog_streambuffer	_buffer_info  (LogLevel_Info);
wlog_streambuffer	_buffer_debug (LogLevel_Debug);


// implement the logging streams
Log Log::err   (&_buffer_error);
Log Log::warn  (&_buffer_warn);
Log Log::info  (&_buffer_info);
Log Log::debug (&_buffer_debug);




