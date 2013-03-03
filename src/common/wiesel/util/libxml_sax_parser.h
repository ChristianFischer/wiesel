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
#ifndef __WIESEL_UTIL_LIBXML_SAX_PARSER_H__
#define __WIESEL_UTIL_LIBXML_SAX_PARSER_H__

#include <wiesel/wiesel-common.def>
#include <wiesel/wiesel-common-config.h>

#include <wiesel/util/xml_parser.h>


#if WIESEL_SUPPORTS_LIBXML2

namespace wiesel {

	// predeclarations

	class DataBuffer;
	class DataSource;
	class File;


	class WIESEL_COMMON_EXPORT LibXml2SaxParser : public IXmlParser
	{
	public:
		LibXml2SaxParser();
		virtual ~LibXml2SaxParser();

		/**
		 * @brief Parse XML-Data from a DataBuffer object.
		 * @param buffer	The DataBuffer to read from.
		 * @param callback	A callback object which will receive the XML data.
		 * @returns \c true on success.
		 */
		virtual bool parse(DataBuffer *buffer, XmlParserCallback *callback);

		/**
		 * @brief Parse XML-Data from a DataSource object.
		 * @param source	The DataSource to read from.
		 * @param callback	A callback object which will receive the XML data.
		 * @returns \c true on success.
		 */
		virtual bool parse(DataSource *source, XmlParserCallback *callback);

		/**
		 * @brief Parse XML-Data from a File object..
		 * @param file	The file to read from.
		 * @param callback	A callback object which will receive the XML data.
		 * @returns \c true on success.
		 */
		virtual bool parse(File *file, XmlParserCallback *callback);
	};

} /* namespace wiesel */
#endif /* WIESEL_SUPPORTS_LIBXML2 */
#endif /* __WIESEL_UTIL_LIBXML_SAX_PARSER_H__ */
