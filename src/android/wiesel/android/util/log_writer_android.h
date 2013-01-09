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
#ifndef __WIESEL_ANDROID_LOG_WRITER_H__
#define __WIESEL_ANDROID_LOG_WRITER_H__

#include <wiesel/wiesel-android.def>
#include <wiesel/util/log_writer.h>
#include <wiesel/util/log.h>



namespace wiesel {
namespace android {

	/**
	 * @brief A log writer module to write log messages into stdout stream
	 */
	class WIESEL_ANDROID_EXPORT LogWriterAndroid : public ILogWriter
	{
	public:
		LogWriterAndroid();
		virtual ~LogWriterAndroid();

	public:
		virtual bool write(LogLevel level, const std::string &tag, const std::string &message);
	};

} /* namespace android */
} /* namespace wiesel */
#endif /* __WIESEL_UTIL_LOG_WRITER_STDOUT_H__ */
