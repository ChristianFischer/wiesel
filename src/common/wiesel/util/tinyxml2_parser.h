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
#ifndef __WIESEL_UTIL_LIBTINYXML2_PARSER_H__
#define __WIESEL_UTIL_LIBTINYXML2_PARSER_H__

#include <wiesel/wiesel-common.def>
#include <wiesel/wiesel-common-config.h>

#include <wiesel/util/xml_parser.h>


#if WIESEL_SUPPORTS_LIBTINYXML2

namespace wiesel {

	// predeclarations

	class DataBuffer;
	class DataSource;
	class File;


	class WIESEL_COMMON_EXPORT LibTinyXml2Parser : public IXmlParser
	{
	public:
		LibTinyXml2Parser();
		virtual ~LibTinyXml2Parser();

		virtual bool parse(DataSource *source, XmlParserCallback *callback);
	};

} /* namespace wiesel */
#endif /* WIESEL_SUPPORTS_LIBTINYXML2 */
#endif /* __WIESEL_UTIL_LIBTINYXML2_PARSER_H__ */
