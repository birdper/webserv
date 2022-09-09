//
// Created by Aleksei S on 05.09.2022.
//

#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include <iostream>
#include <map>

class HttpMessage {
protected:
	std::string version;
	std::string parseErrorString;
private:
	std::map<std::string, std::string>* headers;

public:
	virtual ~HttpMessage();

	void addHeader(const std::string& line);
	void addHeader(const std::string& key, const std::string& value);
	void clearHeaders();
};

#endif
