//
// Created by Aleksei S on 05.09.2022.
//

#pragma once

#include <iostream>
#include <map>
#include "Utils.hpp"

class HttpMessage {
protected:
	std::string _httpVersion;
    std::string parseErrorString;

private:
	std::map<string, string> _headers;

public:
	virtual ~HttpMessage();

    void addHeader(const string& line);
    void addHeader(const string& key, const string& value);
    string findHeaderValue(const string& key) const;

    void setHttpVersion(const string& httpVersion);
    void setParseErrorString(const string& parseErrorString);
    void setHeaders(const std::map<std::string, std::string>& headers);
    const string& getHttpVersion() const;
    const string& getParseErrorString() const;
    const std::map<string, string>& getHeaders() const;
    void clearHeaders();
};
