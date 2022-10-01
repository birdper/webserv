
#include "DefaultErrorPagesRepo.hpp"

DefaultErrorPagesRepo::DefaultErrorPagesRepo() {

//    TODO выбрать реализацию map<string>, либо vector<enum>
	errors["200"] = "OK";
	errors["201"] = "Created";
	errors["202"] = "Accepted";
	errors["203"] = "Non-Authoritative Information";
	errors["204"] = "No Content";
	errors["301"] = "Moved Permanently";
	errors["302"] = "Found";
	errors["304"] = "Not Modified";
	errors["307"] = "Temporary Redirect";
	errors["400"] = "Bad Request";
	errors["403"] = "Forbidden";
	errors["404"] = "Not Found";
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

/*
	errorsEnum[OK] = "OK";
	errorsEnum[CREATED] = "Created";
	errorsEnum[ACCEPTED] = "Accepted";
	errorsEnum[NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
	errorsEnum[NO_CONTENT] = "No Content";
	errorsEnum[MOVED_PERMANENTLY] = "Moved Permanently";
	errorsEnum[FOUND] = "Found";
	errorsEnum[NOT_MODIFIED] = "Not Modified";
	errorsEnum[TEMPORARY_REDIRECT] = "Temporary Redirect";
	errorsEnum[BAD_REQUEST] = "Bad Request";
	errorsEnum[FORBIDDEN] = "Forbidden";
	errorsEnum[NOT_FOUND] = "Not Found";
	errorsEnum[METHOD_NOT_ALLOWED] = "Method Not Allowed";
	errorsEnum[NOT_ACCEPTABLE] = "Not Acceptable";
	errorsEnum[CONFLICT] = "Conflict";
	errorsEnum[GONE] = "Gone";
	errorsEnum[LENGTH_REQUIRED] = "Length Required";
	errorsEnum[PRECONDITION_FAILED] = "Precondition Failed";
	errorsEnum[REQUEST_ENTITY_TOO_LARGE] = "Request Entity Too Large";
	errorsEnum[REQUEST_URI_TOO_LONG] = "Request-URI Too Long";
	errorsEnum[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
	errorsEnum[REQUESTED_RANGE_NOT_SATISFIABLE] = "Requested Range Not Satisfiable";
	errorsEnum[EXPECTATION_FAILED] = "Expectation Failed";
	errorsEnum[INTERNAL_SERVER_ERROR] = "Internal Server Error";
	errorsEnum[NOT_IMPLEMENTED] = "Not Implemented";
	errorsEnum[BAD_GATEWAY] = "Bad Gateway";
	errorsEnum[SERVICE_UNAVAILABLE] = "Service Unavailable";
	errorsEnum[GATEWAY_TIMEOUT] = "Gateway Timeout";
	errorsEnum[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
*/

}

/*
std::string& RepositoryErrorPages::getContentErrorByEnumCode(EnumErrorCode code) {
	return errorsEnum[code];
}
*/

std::string& DefaultErrorPagesRepo::getContentErrorByCode(const std::string& code) {
	std::map<std::string, std::string>::iterator it = errors.find(code);
	if (it == errors.end())
		return *new std::string("");
	return it->second;
}