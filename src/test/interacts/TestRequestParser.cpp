

#include "RequestParser.hpp"
#include "acutest.h"

void parseWhenIncorrectMethod() {
	RequestParser parser;

	Request* request = parser.parse("GETi /dsad/asd.php HTTP/1.1");

	TEST_CHECK(request->isBadRequest() == true);
}

void parseWhenIncorrectUri() {
	RequestParser parser;

	Request* request = parser.parse("GET  HTTP/1.1");

	TEST_CHECK(request->isBadRequest() == true);
}

void parseWhenIncorrectVersionHttp() {
	RequestParser parser;

	Request* request = parser.parse("GET /dsad/asd.php HTTP/1");

	TEST_CHECK(request->isBadRequest() == true);
}

void parseWhenCorrectStartLine() {
	RequestParser parser;

	Request* request = parser.parse("GET /dsad/asd.php HTTP/1.1\r\nHost: localhost");

	TEST_CHECK(request->isBadRequest() == false);
}

void parseWhenUriIsAsterisk() {
	RequestParser parser;

	Request* request = parser.parse("GET * HTTP/1.1\nHost: localhost");
	TEST_CHECK(request->getUri() == "*");
}


void parseWhenSeparatorCrAndLf() {
	RequestParser parser;

	Request* request = parser.parse("GET / HTTP/1.1\r\nHost: localhost");

	TEST_CHECK(request->isBadRequest() == false);
}

void parseWhenSeparatorLf() {
	RequestParser parser;

	Request* request = parser.parse("GET / HTTP/1.1\nHost: localhost");

	TEST_CHECK(request->isBadRequest() == false);
}

void parseWhenHostHeaderIsMissing() {
	RequestParser parser;

	Request* request = parser.parse("GET / HTTP/1.1\nContent-type: localhost");
	request->findHeaderByName("Host");

	TEST_CHECK(request->findHeaderByName("Host") != "localhost");
}

TEST_LIST = {
		{ "parse() Incorrect HTTP method in the start line",  parseWhenIncorrectMethod },
		{ "parse() Incorrect uri in the start line",          parseWhenIncorrectUri },
		{ "parse() Incorrect HTTP version in the start line", parseWhenIncorrectVersionHttp },
		{ "parse() Asterisk in uri",                          parseWhenUriIsAsterisk },
		{ "parse() Correct start line",                       parseWhenCorrectStartLine },
		{ "parse() When host header is missing",             parseWhenHostHeaderIsMissing},
		{ "parse() parseWhenSeparatorLf",                     parseWhenSeparatorLf},
		{ "parse() parseWhenSeparatorCrAndLf",                parseWhenSeparatorCrAndLf},

		{nullptr, nullptr}
};


