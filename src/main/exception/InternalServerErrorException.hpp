#ifndef WEBSERV_INTERNALSERVERERROREXCEPTION_HPP
#define WEBSERV_INTERNALSERVERERROREXCEPTION_HPP

class InternalServerErrorException : public std::exception
{
public:
    char * what () {
        return "InternalServerErrorException";
    }
};

#endif //WEBSERV_INTERNALSERVERERROREXCEPTION_HPP
