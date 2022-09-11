#ifndef FILENOTFOUNDEXCEPTION_HPP
#define FILENOTFOUNDEXCEPTION_HPP
#include <exception>
#include <iostream>

class FileNotFoundException : public std::exception {
private:
	const std::string& fileName;
public:
	explicit FileNotFoundException(const std::string &fileName);
	const char* what() const throw();
};

#endif
