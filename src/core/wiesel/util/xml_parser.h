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

#include <map>
#include <string>
#include <vector>


namespace wiesel {

	// predeclarations

	class DataBuffer;
	class DataSource;
	class File;


	/**
	 * @brief An utility class to create XML-Parsers for custom XML formats.
	 * An implementation will extend this class and use the callback methods
	 * to read the XML content.
	 */
	class WIESEL_CORE_EXPORT XmlSaxParser
	{
	public:
		/**
		 * @brief Creates a new parser object.
		 */
		XmlSaxParser();

		virtual ~XmlSaxParser();

	public:
		/**
		 * @brief Parse XML-Data from a DataBuffer object.
		 * @param buffer	The DataBuffer to read from.
		 * @returns \c true on success.
		 */
		bool parse(DataBuffer *buffer);

		/**
		 * @brief Parse XML-Data from a DataSource object.
		 * @param source	The DataSource to read from.
		 * @returns \c true on success.
		 */
		bool parse(DataSource *source);

		/**
		 * @brief Parse XML-Data from a File object..
		 * @param file	The file to read from.
		 * @returns \c true on success.
		 */
		bool parse(File *file);

	protected:
		/**
		 * @brief Clear the parser state.
		 */
		void clear();

	// SAX handler callbacks
	public:
		/// an alias type for an element's attributes.
		typedef std::map<std::string,std::string>	Attributes;

		/// an alias type for the stack containing all recent XML elements.
		typedef std::vector<std::string>			ElementStack;

		/**
		 * @brief A new XML element hast started. Will be followed by an onElementClosed event.
		 * @param element		The name of the closed element.
		 * @param attributes	A list containing all attributes of this element.
		 */
		virtual void onElementStarted(const std::string &element, const Attributes &attributes);

		/**
		 * @brief A previously started element was closed.
		 * @param element		The name of the closed element.
		 */
		virtual void onElementClosed(const std::string &element);

		/**
		 * @brief Delivers the text-content of the current element.
		 * @param text			The current element's text content.
		 */
		virtual void onTextContent(const std::string &text);

	public:
		const ElementStack *getElementStack() const;

	private:
		ElementStack	stack;
	};

} /* namespace wiesel */
#endif /* __WIESEL_UTIL_XML_PARSER_H__ */
