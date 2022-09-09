//
// Created by Aleksei S on 05.09.2022.
//

#include "HttpMessage.hpp"

void HttpMessage::addHeader(const std::string& line) {

	size_t delimiterPos = line.find(':');
	if (delimiterPos == std::string::npos) {
		std::cout << "Could not addHeader: " << line.c_str() << std::endl;
		return;
	}
	std::string key = line.substr(0, delimiterPos);
	std::string value = line.substr(delimiterPos + 1, line.size() - delimiterPos - 1);

	// Skip all leading spaces in the value
	int i = 0;
	while (i < value.size() && value.at(i) == 0x20) {
		i++;
	}
	value = value.substr(i, value.size());

	// Add header to the map
	addHeader(key, value);
}

void HttpMessage::addHeader(const std::string& key, const std::string& value) {
	headers->insert(std::pair<std::string, std::string>(key, value));
}

HttpMessage::~HttpMessage() {
	headers->clear();
	delete headers;
}

void HttpMessage::clearHeaders() {
	headers->clear();
}
