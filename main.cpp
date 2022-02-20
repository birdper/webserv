#include <iostream>
#include <sstream>

#include <map>
#include <vector>

#include "Tokenizer.hpp"
#include "ParserConfig.hpp"
#include "SettingsStorage.hpp"
#include "SettingsRepositoryImpl.hpp"



void testTokenizer(const std::string& input) {
	Tokenizer tokenizer;

	std::istringstream iss(input);
	std::vector<Token> tokens = tokenizer.tokenize(iss);
	for (int i = 0; i < tokens.size(); ++i) {
		std::cout << "type " << tokens[i].typeName;
		std::cout << " content " << tokens[i].content << std::endl;
	}
}

void testParserConfig(const std::string& input) {
	parser::ParserConfig parser;
	SettingsStorage* storage = new SettingsStorage();

	parser.parseConfig(input, storage);
	SettingsRepositoryImpl repository(storage);
	VirtualServer virtualServer = *storage->findVirtualServerByHost(
			"8080",
			"");
	Config* config = repository.getConfig("/www/html", "8080", "");
	if (config)
		std::cout << "root: " << config->getRoot() << std::endl;
	else
		std::cout << "Not Found Config" << std::endl;


}

size_t parseUriRequest(const std::string& uriRequest,
					   const std::string& uriLocation) {
	return uriRequest.find(uriLocation, 0);
//	return uriLocation.find(uriRequest, 0);
//	size_t posDot = uri.rfind('.');
//	size_t posSlash = uri.rfind('/');
}


int main() {
	std::string input = "server {\n"
						"        listen 8080;\n"
						"\n"
						"\n"
						"        location /www {\n"
						"            error_page 404 /an404.html;\n"
						"            root serv1_loc_www;\n"
						"            client_max_body_size       10m;\n"
						"        }\n"
						"\n"
						"        location /www/htmll {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv1_loc_/www/html;\n"
						"        }\n"
						"\n"
						"        location /www/html/index {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv1_loc_/www/html/index;\n"
						"        }\n"
						"}\n"
						"\n"
						"server {\n"
						"        listen 8080;\n"
						"        server_name hh.ru www.hh.ru;\n"
						"\n"
						"\n"
						"        location /www {\n"
						"            error_page 404 /an404.html;\n"
						"            root serv2_loc_www;\n"
						"            client_max_body_size       10m;\n"
						"        }\n"
						"\n"
						"        location /www/2html {\n"
						"            error_page  404  /404.html;\n"
						"            root  serv2_loc_2html;\n"
						"        }\n"
						"}";
	testParserConfig(input);
//	testTokenizer();




//	std::cout << parseUriRequest("/first/sec/file.ext",
//								 "/sec")	<< std::endl;
	return 0;
}
