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
#include "log_writer.h"

#include <wiesel/module_registry.h>

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>

#include <sstream>
#include <string>
#include <list>


using namespace wiesel;
using namespace std;


// set the highest log level per default.
LogLevel Log::current_log_level = LogLevel_Debug;



/**
 * @brief A simple object conrtaining a single log entry, including level, tag and message.
 */
struct LogEntry {
	LogLevel	level;
	std::string	tag;
	std::string	message;
};

/// stores log messages, when no log writer is available
static std::list<LogEntry> log_cache;


/// try to write a single log message using all available modules.
static bool __log_message(LogLevel level, const std::string &tag, const std::string &message) {
	bool success = false;

	std::vector<ModuleLoader<ILogWriter>*> loaders = ModuleRegistry::getInstance()->findModules<ILogWriter>();
	for(std::vector<ModuleLoader<ILogWriter>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		ILogWriter *writer = (*it)->create();

		if (writer) {
			success |= writer->write(level, tag, message);
		}
	}

	return success;
}


/// try to write a log message via log writer modules, or add it into the cache, if no logging is possible
static void __log_or_cache_message(LogLevel level, const std::string &tag, const std::string &message) {
	if (__log_message(level, tag, message) == false) {
		LogEntry entry = { level, tag, message };
		log_cache.push_back(entry);
	}

	return;
}


/// try to write all pending messages to streams. returns true, when the cache is empty
static bool __flush_cache() {
	for(std::list<LogEntry>::iterator it=log_cache.begin(); it!=log_cache.end();) {
		bool success = __log_message(it->level, it->tag, it->message);
		if (success) {
			it = log_cache.erase(it);
		}
		else {
			// stop, when an error occured
			break;
		}
	}

	return log_cache.empty();
}



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
		size_t buffer_size = 10240;
		char buffer[buffer_size];

		va_list args;
		va_start(args, message);
		vsnprintf(buffer, buffer_size, message, args);
		va_end(args);

		// write into streams
		std::string str_message(buffer);
		__log_or_cache_message(level, tag, str_message);

		// return the number of written characters
		return str_message.size();
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
	void writeCurrentLine() {
		// at first, try to flush the cache
		__flush_cache();

		// try to write the current message
		__log_or_cache_message(level, tag, ss.str());

		// clear the current buffer
		ss.str("");
		ss.clear();

		return;
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




