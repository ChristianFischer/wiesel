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

#include <wiesel/io/datasource.h>
#include <wiesel/io/file.h>
#include <wiesel/module_registry.h>

using namespace wiesel;



XmlParser::XmlParser() {
	this->callback	= NULL;
	this->state		= NULL;
	return;
}


XmlParser::XmlParser(XmlParserCallback *callback) {
	assert(callback);

	this->callback	= keep(callback);
	this->state		= NULL;

	return;
}


XmlParser::~XmlParser() {
	assert(state == NULL); // parsing was not finished?

	clear_ref(callback);
	clear_ref(state);

	return;
}


bool XmlParser::parse(DataSource *source, XmlParserCallback *callback) {
	std::vector<ModuleLoader<IXmlParser>*> loaders = ModuleRegistry::getInstance()->findModules<IXmlParser>();
	for(std::vector<ModuleLoader<IXmlParser>*>::iterator it=loaders.begin(); it!=loaders.end(); it++) {
		IXmlParser *parser = (*it)->create();

		if (parser) {
			bool success = parser->parse(source, callback);
			if (success) {
				return true;
			}
		}
	}

	return false;
}


void XmlParser::start() {
	assert(state == NULL); // last parsing was not finished?

	if (state) {
		clear_ref(state);
	}

	state = new XmlDocumentState();
	keep(state);

	if (callback) {
		callback->onDocumentStarted(state);
	}

	return;
}


void XmlParser::finish() {
	assert(state);					// parsing not started?
	assert(state->stack.empty());	// mismatch between opening and closing tags?

	if (state) {
		if (callback) {
			callback->onDocumentFinished(state);
		}

		clear_ref(state);
	}

	return;
}


void XmlParser::startElement(const std::string &element, const Attributes &attributes) {
	assert(state);

	if (state) {
		state->stack.push_back(element);

		if (callback) {
			callback->onElementStarted(state, element, attributes);
		}
	}

	return;
}


void XmlParser::closeElement(const std::string &element) {
	assert(state);

	if (state) {
		assert(state->getCurrentElement() == element);

		if (state->getCurrentElement() == element) {
			state->stack.pop_back();
		}

		if (callback) {
			callback->onElementClosed(state, element);
		}
	}

	return;
}


void XmlParser::addTextContent(const std::string &text) {
	assert(state);

	if (state) {
		if (callback) {
			callback->onTextContent(state, text);
		}
	}

	return;
}





XmlDocumentState::XmlDocumentState() {
	return;
}

XmlDocumentState::~XmlDocumentState() {
	return;
}


std::string XmlDocumentState::getCurrentElement() const {
	if (stack.size()) {
		return stack.back();
	}

	return "";
}


const XmlParser::ElementStack *XmlDocumentState::getElementStack() const {
	return &stack;
}




IXmlParser::IXmlParser() {
	return;
}

IXmlParser::~IXmlParser() {
	return;
}




XmlParserCallback::XmlParserCallback() {
	return;
}

XmlParserCallback::~XmlParserCallback() {
	return;
}

void XmlParserCallback::onDocumentStarted(const XmlDocumentState*) {
	return;
}

void XmlParserCallback::onDocumentFinished(const XmlDocumentState*) {
	return;
}

void XmlParserCallback::onElementStarted(const XmlDocumentState*, const std::string&, const XmlParser::Attributes&) {
	return;
}

void XmlParserCallback::onElementClosed(const XmlDocumentState*, const std::string&) {
	return;
}

void XmlParserCallback::onTextContent(const XmlDocumentState*, const std::string&) {
	return;
}

