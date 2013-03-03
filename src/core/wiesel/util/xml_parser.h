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
#ifndef __WIESEL_UTIL_XML_PARSER_H__
#define __WIESEL_UTIL_XML_PARSER_H__

#include <wiesel/wiesel-core.def>

#include <wiesel/util/shared_object.h>
#include <wiesel/module.h>

#include <map>
#include <string>
#include <vector>


namespace wiesel {

	// predeclarations

	class DataBuffer;
	class DataSource;

	class XmlParser;
	class XmlParserCallback;
	class XmlDocumentState;



	/**
	 * @brief An utility class to create XML-Parsers for custom XML formats.
	 */
	class WIESEL_CORE_EXPORT XmlParser : public virtual SharedObject
	{
	public:
		/// an alias type for the stack containing all recent XML elements.
		typedef std::vector<std::string>			ElementStack;

		/// an alias type for an element's attributes.
		typedef std::map<std::string,std::string>	Attributes;

	private:
		XmlParser();

	public:
		XmlParser(XmlParserCallback *callback);
		virtual ~XmlParser();

	public:
		/**
		 * @brief Parse XML-Data from a DataSource object.
		 * @param source	The DataSource to read from.
		 * @param callback	A callback object which will receive the XML data.
		 * @returns \c true on success.
		 */
		static bool parse(DataSource *source, XmlParserCallback *callback);

	public:
		/// start parsing the xml data
		void start();

		/// finish parsing.
		void finish();

		/// a new XML element started.
		void startElement(const std::string &element, const Attributes &attributes);

		/// closes the current XML element. \c name should be the same as the top-level element.
		void closeElement(const std::string &element);

		/// add text content for the current XML element
		void addTextContent(const std::string &text);

	private:
		XmlParserCallback*		callback;
		XmlDocumentState*		state;
	};



	/**
	 * @brief A module to implement an XML parser,
	 * which sends its results to the given callback object.
	 */
	class WIESEL_CORE_EXPORT IXmlParser : public Module
	{
	public:
		IXmlParser();
		virtual ~IXmlParser();

		/**
		 * @brief Parse XML-Data from a DataSource object.
		 * @param source	The DataSource to read from.
		 * @param callback	A callback object which will receive the XML data.
		 * @returns \c true on success.
		 */
		virtual bool parse(DataSource *source, XmlParserCallback *callback) = 0;
	};



	/**
	 * @brief An object which stores the current state of a document during parsing.
	 */
	class WIESEL_CORE_EXPORT XmlDocumentState : public virtual SharedObject
	{
	friend class XmlParser;

	public:
		XmlDocumentState();
		virtual ~XmlDocumentState();

	public:

	public:
		/// get the tag-name of the current XML element.
		std::string getCurrentElement() const;

		/// get the stack with the current XML element and all of it's parents.
		const XmlParser::ElementStack *getElementStack() const;

	private:
		XmlParser::ElementStack			stack;
	};



	/**
	 * @brief The callback class to implement a parser for custom XML formats.
	 * An implementation will extend this class and use the callback methods
	 * to read the XML content.
	 */
	class WIESEL_CORE_EXPORT XmlParserCallback : public virtual SharedObject
	{
	protected:
		XmlParserCallback();

	public:
		virtual ~XmlParserCallback();

	protected:
		/**
		 * @brief Clear the parser state.
		 */
		void clear();

	// SAX handler callbacks
	public:
		/**
		 * @brief Started parsing a new XML Document.
		 * @param state			The state of the new XML document.
		 */
		virtual void onDocumentStarted(const XmlDocumentState *state);

		/**
		 * @brief Parsing the current document has been finished.
		 * @param state			The state of the new XML document.
		 */
		virtual void onDocumentFinished(const XmlDocumentState *state);

		/**
		 * @brief A new XML element hast started. Will be followed by an onElementClosed event.
		 * @param state			The current state of the parsed XML document.
		 * @param element		The name of the closed element.
		 * @param attributes	A list containing all attributes of this element.
		 */
		virtual void onElementStarted(const XmlDocumentState *state, const std::string &element, const XmlParser::Attributes &attributes);

		/**
		 * @brief A previously started element was closed.
		 * @param state			The current state of the parsed XML document.
		 * @param element		The name of the closed element.
		 */
		virtual void onElementClosed(const XmlDocumentState *state, const std::string &element);

		/**
		 * @brief Delivers the text-content of the current element.
		 * @param state			The current state of the parsed XML document.
		 * @param text			The current element's text content.
		 */
		virtual void onTextContent(const XmlDocumentState *state, const std::string &text);
	};

} /* namespace wiesel */
#endif /* __WIESEL_UTIL_XML_PARSER_H__ */
