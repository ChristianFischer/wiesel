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
#include "xml_parser.h"
#include "wiesel/io/datasource.h"
#include "wiesel/io/file.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlversion.h>

#include <algorithm>



using namespace wiesel;



struct ParserData {
	XmlSaxParser				*callbacks;
	XmlSaxParser::ElementStack	*stack;
};


static void handle_startElement(void *user_data, const xmlChar *name, const xmlChar **attribs) {
	ParserData *parser = reinterpret_cast<ParserData*>(user_data);
	XmlSaxParser::Attributes attribute_list;
	std::string element = reinterpret_cast<const char*>(name);

	// get all the attributes
	while(attribs && attribs[0]) {
		std::string attr_name  = reinterpret_cast<const char*>(attribs[0]);
		std::string attr_value = reinterpret_cast<const char*>(attribs[1]);

		attribute_list[attr_name] = attr_value;

		attribs += 2;
	}

	// push the new element to the stack
	parser->stack->push_back(element);

	// invoke callback
	parser->callbacks->onElementStarted(element, attribute_list);

	return;
}


static void handle_endElement(void *user_data, const xmlChar *name) {
	ParserData *parser = reinterpret_cast<ParserData*>(user_data);
	std::string element = reinterpret_cast<const char*>(name);

	// does the element name match the stack?
	assert(parser->stack->back() == element);

	// invoke callback
	parser->callbacks->onElementClosed(element);

	// remove this element from the stack
	if (parser->stack->size() && parser->stack->back() == element) {
		parser->stack->pop_back();
	}

	return;
}


static void handle_text(void *user_data, const xmlChar *chars, int len) {
	ParserData *parser = reinterpret_cast<ParserData*>(user_data);
	std::string text(reinterpret_cast<const char*>(chars), len);

	// invoke callback
	parser->callbacks->onTextContent(text);

	return;
}



static void init_sax_handler(xmlSAXHandler &handler) {
	handler.startElement		= &handle_startElement;
	handler.endElement			= &handle_endElement;
	handler.characters			= &handle_text;

	return;
}



XmlSaxParser::XmlSaxParser() {
	return;
}


XmlSaxParser::~XmlSaxParser() {
	return;
}


bool XmlSaxParser::parse(DataBuffer *buffer) {
	assert(buffer);
	if (buffer == NULL) {
		return false;
	}

	LIBXML_TEST_VERSION
	clear();

	ParserData parser_data = { this, &this->stack };
	xmlSAXHandler handler = {0};
	init_sax_handler(handler);

	// start parsing
	int result = xmlSAXUserParseMemory(&handler, &parser_data, buffer->getDataAsCharPtr(), buffer->getSize());

	// cleanup
	xmlCleanupParser();
	xmlMemoryDump();

	return result == 0;
}


bool XmlSaxParser::parse(DataSource *source) {
	FileDataSource *fds = dynamic_cast<FileDataSource*>(source);
	if (fds) {
		return parse(fds->getFile());
	}
	else {
		DataBuffer *buffer = source->getDataBuffer();
		if (buffer) {
			return parse(buffer);
		}
	}

	return false;
}


bool XmlSaxParser::parse(File* file) {
	std::string native_path = file->getNativePath();
	if (native_path.empty() == false) {
		LIBXML_TEST_VERSION
		clear();

		ParserData parser_data = { this, &this->stack };
		xmlSAXHandler handler = {0};
		init_sax_handler(handler);

		// start parsing
		int result = xmlSAXUserParseFile(&handler, &parser_data, native_path.c_str());

		// cleanup
		xmlCleanupParser();
		xmlMemoryDump();

		if (result == 0) {
			return true;
		}
	}
	
	// when file parsing fails... there's still a DataBuffer...
	DataBuffer *buffer = file->asDataSource()->getDataBuffer();
	if (buffer) {
		return parse(buffer);
	}

	return false;
}


void XmlSaxParser::clear() {
	stack.clear();
}


void XmlSaxParser::onElementStarted(const std::string&, const Attributes&) {
	return;
}

void XmlSaxParser::onElementClosed(const std::string&) {
	return;
}

void XmlSaxParser::onTextContent(const std::string&) {
	return;
}

