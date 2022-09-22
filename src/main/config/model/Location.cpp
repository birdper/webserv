//
// Created by nbirdper on 28.01.2022.
//

#include "Location.hpp"

Location::Location(const std::string& uri,
                   const Parameters& parentParams) {

    _params = parentParams;
    _params.uri = uri;
}

Location::Location(Location& location) :
        _params(location._params) {
}

Location& Location::operator=(const Location& other) {
    if (this != &other) {
        _params = other._params;
    }
    return *this;
}

Parameters& Location::getParameters() {
	return _params;
}

std::string Location::getUri() const {
	return _params.uri;
}
