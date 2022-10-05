
#include "DefaultStatusCodeRepo.hpp"

DefaultStatusCodeRepo::DefaultStatusCodeRepo() {
	errors["200"] = "OK";
	errors["201"] = "Created";
	errors["202"] = "Accepted";
	errors["203"] = "Non-Authoritative Information";
	errors["204"] = "No Content";
	errors["301"] = "Moved Permanently";
	errors["302"] = "Found";
	errors["303"] = "See Other";
	errors["304"] = "Not Modified";
	errors["307"] = "Temporary Redirect";
	errors["308"] = "Permanent Redirect";
	errors["400"] = "Bad Request";
	errors["403"] = "Forbidden";
	errors["404"] = "Page Not Found";
	errors["405"] = "Method Not Allowed";
	errors["406"] = "Not Acceptable";
	errors["409"] = "Conflict";
	errors["410"] = "Gone";
	errors["411"] = "Length Required";
	errors["412"] = "Precondition Failed";
	errors["413"] = "Request Entity Too Large";
	errors["414"] = "Request-URI Too Long";
	errors["415"] = "Unsupported Media Type";
	errors["416"] = "Requested Range Not Satisfiable";
	errors["417"] = "Expectation Failed";
	errors["500"] = "Internal Server Error";
	errors["501"] = "Not Implemented";
	errors["502"] = "Bad Gateway";
	errors["503"] = "Service Unavailable";
	errors["504"] = "Gateway Timeout";
	errors["505"] = "HTTP Version Not Supported";
}

std::string& DefaultStatusCodeRepo::getDescriptionByCode(const std::string& code) {
	std::map<std::string, std::string>::iterator it = errors.find(code);
	if (it == errors.end())
		return *new std::string("");
	return it->second;
}
