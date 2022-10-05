
#include "FileNotFoundException.hpp"

FileNotFoundException::FileNotFoundException(const std::string& cause) :
        _cause(cause) {
}

const char* FileNotFoundException::what() const throw() {
    return _cause.c_str();
}

FileNotFoundException::~FileNotFoundException() throw() {

}
