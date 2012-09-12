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
#ifndef __WIESEL_UTIL_LOG_H__
#define __WIESEL_UTIL_LOG_H__

#include <wiesel/wiesel-common.def>

#include <ostream>
#include <string>


namespace wiesel {

	/// define the default log tag
	#ifndef WIESEL_LOG_TAG
	#define WIESEL_LOG_TAG		"wiesel"
	#endif


	/**
	 * @brief definitions of different logging levels.
	 */
	enum LogLevel {
		LogLevel_None,
		LogLevel_Error,
		LogLevel_Warning,
		LogLevel_Info,
		LogLevel_Debug,
	};



	/**
	 * @brief write a single log message to the output console, if the current log level is high enough.
	 * @param level		The current log level.
	 */
	WIESEL_COMMON_EXPORT int logmsg(LogLevel level, const char *tag, const char *message, ...)
	#if defined(__GNUC__)
		__attribute__ ((format(printf, 3, 4)))
	#endif
	;



	/**
	 * @brief A Log class for logging messages using a std::ostream derived class.
	 */
	class WIESEL_COMMON_EXPORT Log : public std::basic_ostream< char, std::char_traits<char> >
	{
	public:
		Log(std::basic_streambuf<char, std::char_traits<char> > *buffer);
		virtual ~Log();

	public:
		static WIESEL_COMMON_EXPORT Log err;		//!< error log level for critical error messages.
		static WIESEL_COMMON_EXPORT Log warn;		//!< warning log level for minor errors and warnings.
		static WIESEL_COMMON_EXPORT Log info;		//!< info log level for simple program information.
		static WIESEL_COMMON_EXPORT Log debug;		//!< detailed log level for detailed program information, not relevant for productive usage.


		/**
		 * @brief set the current \ref LogLevel.
		 * All messages with a higher level than the currently configured level will be discarded.
		 */
		static WIESEL_COMMON_EXPORT void setLevel(LogLevel level);

		/**
		 * @brief get the current \ref LogLevel.
		 */
		static inline LogLevel getLevel() {
			return current_log_level;
		}

		/**
		 * @brief checks, if a specific combination of log tag and \ref LogLevel will be included in the log messages.
		 */
		static WIESEL_COMMON_EXPORT bool isLogged(LogLevel level, const char *tag);

		/**
		 * @brief checks, if a specific combination of log tag and \ref LogLevel will be included in the log messages.
		 */
		inline static bool isLogged(LogLevel level, const std::string &tag) {
			return isLogged(level, tag.c_str());
		}

	private:
		static WIESEL_COMMON_EXPORT LogLevel		current_log_level;
	};

} /* namespace wiesel */
#endif /* __WIESEL_UTIL_LOG_H__ */
