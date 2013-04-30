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
#include "libxml_sax_parser.h"

#if WIESEL_SUPPORTS_LIBXML2

#include <wiesel/io/databuffer.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/util/log.h>


#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlversion.h>

#include <algorithm>


using namespace wiesel;



static void handle_startElement(void *user_data, const xmlChar *name, const xmlChar **attribs) {
	XmlParser *parser = reinterpret_cast<XmlParser*>(user_data);
	XmlParser::Attributes attribute_list;
	std::string element = reinterpret_cast<const char*>(name);

	// get all the attributes
	while(attribs && attribs[0]) {
		std::string attr_name  = reinterpret_cast<const char*>(attribs[0]);
		std::string attr_value = reinterpret_cast<const char*>(attribs[1]);

		attribute_list[attr_name] = attr_value;

		attribs += 2;
	}

	// invoke callback
	parser->startElement(element, attribute_list);

	return;
}


static void handle_endElement(void *user_data, const xmlChar *name) {
	XmlParser *parser = reinterpret_cast<XmlParser*>(user_data);
	std::string element = reinterpret_cast<const char*>(name);

	// invoke callback
	parser->closeElement(element);

	return;
}


static void handle_text(void *user_data, const xmlChar *chars, int len) {
	XmlParser *parser = reinterpret_cast<XmlParser*>(user_data);
	std::string text(reinterpret_cast<const char*>(chars), len);

	// invoke callback
	parser->addTextContent(text);

	return;
}



static void init_sax_handler(xmlSAXHandler &handler) {
	handler.startElement		= &handle_startElement;
	handler.endElement			= &handle_endElement;
	handler.characters			= &handle_text;

	return;
}



LibXml2SaxParser::LibXml2SaxParser() {
	return;
}


LibXml2SaxParser::~LibXml2SaxParser() {
	return;
}


bool LibXml2SaxParser::parse(DataBuffer *buffer, XmlParserCallback *callback) {
	assert(buffer);
	if (buffer == NULL) {
		return false;
	}

	LIBXML_TEST_VERSION
	xmlSAXHandler handler = {0};
	init_sax_handler(handler);

	// create and initialize parser object
	XmlParser *parser = keep(new XmlParser(callback));
	parser->start();

	// start parsing
	int result = xmlSAXUserParseMemory(&handler, parser, buffer->getDataAsCharPtr(), buffer->getSize());

	// parser cleanup
	parser->finish();
	safe_release(parser);

	// libxml cleanup
	xmlCleanupParser();
	xmlMemoryDump();

	return result == 0;
}


bool LibXml2SaxParser::parse(DataSource *source, XmlParserCallback *callback) {
	FileDataSource *fds = dynamic_cast<FileDataSource*>(source);
	if (fds) {
		return parse(fds->getFile(), callback);
	}
	else {
		DataBuffer *buffer = source->getDataBuffer();
		if (buffer) {
			return parse(buffer, callback);
		}
	}

	return false;
}


bool LibXml2SaxParser::parse(File* file, XmlParserCallback *callback) {
	std::string native_path = file->getNativePath();
	if (native_path.empty() == false) {
		LIBXML_TEST_VERSION
		xmlSAXHandler handler = {0};
		init_sax_handler(handler);

		// create and initialize parser object
		XmlParser *parser = keep(new XmlParser(callback));
		parser->start();

		// start parsing
		int result = xmlSAXUserParseFile(&handler, parser, native_path.c_str());

		// parser cleanup
		parser->finish();
		safe_release(parser);

		// libxml cleanup
		xmlCleanupParser();
		xmlMemoryDump();

		if (result == 0) {
			return true;
		}
	}
	
	// when file parsing fails... there's still a DataBuffer...
	DataBuffer *buffer = file->asDataSource()->getDataBuffer();
	if (buffer) {
		return parse(buffer, callback);
	}

	return false;
}

#endif /* WIESEL_SUPPORTS_LIBXML2 */
