#ifndef WEBSERV_CGIEXCEPTION_HPP
#define WEBSERV_CGIEXCEPTION_HPP

#include <iostream>
#include <exception>

class CGIException : public std::exception {
private:
    const std::string& _cause;
public:
    explicit CGIException(const std::string& cause);
    const char* what() const throw();
};

#endif //WEBSERV_CGIEXCEPTION_HPP
