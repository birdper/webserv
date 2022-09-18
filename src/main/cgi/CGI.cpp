#include "CGI.hpp"

CGI::CGI(const string& pathToCGI, const string& extension,
    const string& root, const string& ip, const string& port)
: m_pathToCGI(pathToCGI), m_extension(extension), m_root(root), m_ip(ip), m_port(port) {}

CGI::~CGI() {}

