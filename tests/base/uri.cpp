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
#include "gtest/gtest.h"

#include <wiesel/io/uri.h>

using namespace wiesel;



/**
 * Parse an uri with a host only.
 */
TEST(Uri, Parse_Host) {
	std::string original_uri = "www.sample.com";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("",									uri.getScheme());
	EXPECT_EQ("",									uri.getUserInfo());
	EXPECT_EQ("",									uri.getUserName());
	EXPECT_EQ("",									uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ("www.sample.com",						uri.getAuthority());
	EXPECT_EQ("",									uri.getPath());
	EXPECT_EQ("",									uri.getQueryString());
	EXPECT_EQ("",									uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with host and port.
 */
TEST(Uri, Parse_HostPort) {
	std::string original_uri = "www.sample.com:80";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_TRUE(uri.hasPort());

	EXPECT_EQ("",									uri.getScheme());
	EXPECT_EQ("",									uri.getUserInfo());
	EXPECT_EQ("",									uri.getUserName());
	EXPECT_EQ("",									uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ(80u,									uri.getPort());
	EXPECT_EQ("www.sample.com:80",					uri.getAuthority());
	EXPECT_EQ("",									uri.getPath());
	EXPECT_EQ("",									uri.getQueryString());
	EXPECT_EQ("",									uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with scheme and authority.
 */
TEST(Uri, Parse_SchemeAuthority) {
	std::string original_uri = "http://www.sample.com";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("",									uri.getUserInfo());
	EXPECT_EQ("",									uri.getUserName());
	EXPECT_EQ("",									uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ("www.sample.com",						uri.getAuthority());
	EXPECT_EQ("",									uri.getPath());
	EXPECT_EQ("",									uri.getQueryString());
	EXPECT_EQ("",									uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with scheme and authority with userinfo.
 */
TEST(Uri, Parse_SchemeAuthorityUserPort) {
	std::string original_uri = "http://myname:secret@www.sample.com:8080";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_TRUE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("myname:secret",						uri.getUserInfo());
	EXPECT_EQ("myname",								uri.getUserName());
	EXPECT_EQ("secret",								uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ(8080u,								uri.getPort());
	EXPECT_EQ("myname:secret@www.sample.com:8080",	uri.getAuthority());
	EXPECT_EQ("",									uri.getPath());
	EXPECT_EQ("",									uri.getQueryString());
	EXPECT_EQ("",									uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with scheme and authority with userinfo and a query.
 */
TEST(Uri, Parse_SchemeAuthorityUserPortQuery) {
	std::string original_uri = "http://myname:secret@www.sample.com:8080?right=left&up=down";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_TRUE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("myname:secret",						uri.getUserInfo());
	EXPECT_EQ("myname",								uri.getUserName());
	EXPECT_EQ("secret",								uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ(8080u,								uri.getPort());
	EXPECT_EQ("myname:secret@www.sample.com:8080",	uri.getAuthority());
	EXPECT_EQ("",									uri.getPath());
	EXPECT_EQ("right=left&up=down",					uri.getQueryString());
	EXPECT_EQ("left",								uri.getQueryArg("right"));
	EXPECT_EQ("down",								uri.getQueryArg("up"));
	EXPECT_EQ("",									uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with scheme, authority, path, query and fragment.
 */
TEST(Uri, Parse_SchemeAuthorityPathQueryFragment) {
	std::string original_uri = "http://myname:secret@www.sample.com:8080/path/to/index.html?empty=&param=value#anchor";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_TRUE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("myname:secret",						uri.getUserInfo());
	EXPECT_EQ("myname",								uri.getUserName());
	EXPECT_EQ("secret",								uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ(8080u,								uri.getPort());
	EXPECT_EQ("myname:secret@www.sample.com:8080",	uri.getAuthority());
	EXPECT_EQ("/path/to/index.html",				uri.getPath());
	EXPECT_EQ("empty=&param=value",					uri.getQueryString());
	EXPECT_EQ("",									uri.getQueryArg("empty"));
	EXPECT_EQ("value",								uri.getQueryArg("param"));
	EXPECT_EQ("anchor",								uri.getFragment());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with an empty query and fragment.
 */
TEST(Uri, Parse_SchemeAuthorityEmptyQueryFragment) {
	std::string original_uri = "http://www.sample.com/path/to/index.html?#";
	std::string expected_uri = "http://www.sample.com/path/to/index.html";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("",									uri.getUserInfo());
	EXPECT_EQ("",									uri.getUserName());
	EXPECT_EQ("",									uri.getPassword());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ(0u,									uri.getPort());
	EXPECT_EQ("www.sample.com",						uri.getAuthority());
	EXPECT_EQ("/path/to/index.html",				uri.getPath());
	EXPECT_EQ("",									uri.getQueryString());
	EXPECT_EQ("",									uri.getFragment());

	// toString will be different to the parsed original, because query and fragment are empty
	EXPECT_EQ(expected_uri,							uri.toString());
}


/**
 * Parse an uri with a file URI.
 */
TEST(Uri, Parse_FileUri) {
	std::string original_uri = "file:///var/log/messages";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("file",								uri.getScheme());
	EXPECT_EQ("",									uri.getAuthority());
	EXPECT_EQ("/var/log/messages",					uri.getPath());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Parse an uri with a file URI with a windows-style path.
 */
TEST(Uri, Parse_FileUriWindows) {
	std::string original_uri = "file:///C:/Windows/system32";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("file",								uri.getScheme());
	EXPECT_EQ("",									uri.getAuthority());
	EXPECT_EQ("/C:/Windows/system32",				uri.getPath());
	EXPECT_EQ(original_uri,							uri.toString());
}


/**
 * Override an URI's query arguments.
 */
TEST(Uri, AlterQueryArguments) {
	std::string original_uri = "http://www.sample.com/index.html?arg1=a";
	URI uri = URI::parse(original_uri);

	EXPECT_FALSE(uri.empty());
	EXPECT_FALSE(uri.hasPort());

	EXPECT_EQ("http",								uri.getScheme());
	EXPECT_EQ("www.sample.com",						uri.getHost());
	EXPECT_EQ("www.sample.com",						uri.getAuthority());
	EXPECT_EQ("/index.html",						uri.getPath());
	EXPECT_EQ("arg1=a",								uri.getQueryString());
	EXPECT_EQ("a",									uri.getQueryArg("arg1"));
	EXPECT_EQ(original_uri,							uri.toString());

	// alter the existing argument
	uri.setQueryArg("arg1",		"b");
	EXPECT_EQ("arg1=b",								uri.getQueryString());
	EXPECT_EQ("b",									uri.getQueryArg("arg1"));

	// add another argument
	uri.setQueryArg("arg2",		"q");
	EXPECT_EQ("arg1=b&arg2=q",						uri.getQueryString());
	EXPECT_EQ("b",									uri.getQueryArg("arg1"));
	EXPECT_EQ("q",									uri.getQueryArg("arg2"));

	// alter the 1st one again
	uri.setQueryArg("arg1",		"");
	EXPECT_EQ("arg1=&arg2=q",						uri.getQueryString());
	EXPECT_EQ("",									uri.getQueryArg("arg1"));
	EXPECT_EQ("q",									uri.getQueryArg("arg2"));
}
