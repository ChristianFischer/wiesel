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
#ifndef __WIESEL_IO_NET_CONNECTION_DATASOURCE_H__
#define __WIESEL_IO_NET_CONNECTION_DATASOURCE_H__

#include <wiesel/io/datasource.h>
#include <wiesel/io/uri.h>
#include <wiesel/wiesel-net.def>

namespace wiesel {

	/**
	 * @brief A datasource from any connection.
	 */
	class WIESEL_NET_EXPORT ConnectionDataSource : public DataSource
	{
	private:
		ConnectionDataSource();

	public:
		ConnectionDataSource(const URI& uri);
		virtual ~ConnectionDataSource();

	public:
		virtual DataBuffer *getDataBuffer();
		virtual void releaseDataBuffer();

		/**
		 * @brief Get the URI where the content will be loaded from.
		 */
		inline const URI& getURI() const {
			return uri;
		}

	private:
		URI					uri;
		ref<DataBuffer>		content;
	};

}

#endif // __WIESEL_IO_NET_CONNECTION_DATASOURCE_H__

