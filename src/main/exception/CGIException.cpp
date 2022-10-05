#include "CGIException.hpp"

CGIException::CGIException(const std::string& cause)
: _cause(cause) {}

const char* CGIException::what() const throw() {
    return _cause.c_str();
}

CGIException::~CGIException() throw() {

}
