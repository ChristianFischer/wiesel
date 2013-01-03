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
#ifndef __WIESEL_UTIL_LOG_WRITER_H__
#define __WIESEL_UTIL_LOG_WRITER_H__

#include <wiesel/wiesel-core.def>
#include <wiesel/module.h>

#include "log.h"


namespace wiesel {

	/**
	 * @brief A module to write log messages into a stream or file
	 */
	class WIESEL_CORE_EXPORT ILogWriter : public Module
	{
	public:
		ILogWriter();
		virtual ~ILogWriter();

	public:
		/**
		 * @brief Writes a message into the module's log.
		 * @param level		the current logging level.
		 * @param tag		the message's log tag.
		 * @param message	the message to log.
		 * @return \c true on success, \c false otherwise.
		 */
		virtual bool write(LogLevel level, const std::string &tag, const std::string &message) = 0;
	};

} /* namespace wiesel */
#endif /* __WIESEL_UTIL_LOG_WRITER_H__ */
