//
// Created by Aleksei S on 05.09.2022.
//

#include "HttpMessage.hpp"

HttpMessage::~HttpMessage() {
    _headers.clear();
}

void HttpMessage::setHeaders(const std::map<string, string>& headers) {
    this->_headers = headers;
}

void HttpMessage::addHeader(const string& line) {
	size_t delimiterPos = line.find(':');
	if (delimiterPos == string::npos) {
		cout << "Could not addHeader: " << line.c_str() << endl;
		return;
	}
	string key = line.substr(0, delimiterPos);
	string value = line.substr(delimiterPos + 1, line.size() - delimiterPos - 1);

    value = Utils::ltrim(value, " ");

	addHeader(key, value);
}

void HttpMessage::addHeader(const string& key, const string& value) {
	_headers.insert(std::pair<string, string>(key, value));
}

string HttpMessage::findHeaderValue(const string& key) const {
    std::map<string, string>::const_iterator it = _headers.find(key);

    if (it == _headers.end()) {
        it = _headers.find(Utils::ft_toLower(key));
        if (it == _headers.end())
            return "";
    }
    return it->second;
}

void HttpMessage::clearHeaders() {
	_headers.clear();
}

const string& HttpMessage::getHttpVersion() const {
    return _httpVersion;
}

void HttpMessage::setHttpVersion(const string& httpVersion) {
    HttpMessage::_httpVersion = httpVersion;
}

const std::map<string, string>& HttpMessage::getHeaders() const {
    return _headers;
}
