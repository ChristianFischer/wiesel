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
#include "tinyxml2_parser.h"

#if WIESEL_SUPPORTS_LIBTINYXML2

#include <wiesel/io/databuffer.h>
#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/util/log.h>

#include <tinyxml2.h>

using namespace wiesel;




class LibTinyXml2ParserVisitor : public tinyxml2::XMLVisitor
{
private:
	LibTinyXml2ParserVisitor() {
		return;
	}

public:
	LibTinyXml2ParserVisitor(XmlParserCallback *callback) {
		this->parser = keep(new XmlParser(callback));
	}

	virtual ~LibTinyXml2ParserVisitor() {
		clear_ref(parser);
	}

public:
	virtual bool VisitEnter(const tinyxml2::XMLDocument& doc) {
		parser->start();
		return true;
	}

	virtual bool VisitExit(const tinyxml2::XMLDocument& doc) {
		parser->finish();
		return true;
	}

	virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute) {
		const tinyxml2::XMLAttribute *current_attribute = firstAttribute;
		XmlParser::Attributes attributes;

		while(current_attribute) {
			attributes[current_attribute->Name()] = current_attribute->Value();
			current_attribute = current_attribute->Next();
		}

		parser->startElement(element.Name(), attributes);

		return true;
	}

	virtual bool VisitExit(const tinyxml2::XMLElement& element) {
		parser->closeElement(element.Name());
		return true;
	}

	virtual bool Visit(const tinyxml2::XMLText& text) {
		parser->addTextContent(text.Value());
		return true;
	}

private:
	XmlParser* parser;
};





LibTinyXml2Parser::LibTinyXml2Parser() {
	return;
}


LibTinyXml2Parser::~LibTinyXml2Parser() {
	return;
}


bool LibTinyXml2Parser::parse(DataSource *source, XmlParserCallback *callback) {
	tinyxml2::XMLDocument xml_doc;
	int result = tinyxml2::XML_ERROR_FILE_NOT_FOUND;

	FileDataSource *fds = dynamic_cast<FileDataSource*>(source);
	if (fds) {
		// load the file with tinyxml
		result = xml_doc.LoadFile(fds->getFile()->getNativePath().c_str());
	}
	
	// when file loading failed or not possible, try loading from the buffer
	if (result != tinyxml2::XML_SUCCESS) {
		DataBuffer *buffer = source->getDataBuffer();

		if (buffer) {
			// we need to copy the XML data into a std::string,
			// because the buffer is not null-terminated
			std::string xml_content(buffer->getDataAsCharPtr(), buffer->getSize());

			result = xml_doc.Parse(xml_content.c_str());
		}
	}

	// when the source was parsed successfully, start parsing the result with our own parser
	if (result == tinyxml2::XML_SUCCESS) {
		LibTinyXml2ParserVisitor visitor(callback);
		xml_doc.Accept(&visitor);

		return true;
	}

	return false;
}

#endif /* WIESEL_SUPPORTS_LIBTINYXML2 */
