#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP
#include <map>
#include <string>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "CGIException.hpp"
#include "Request.hpp"
#include "usings.hpp"

class CGI {
private:
    Request&    m_request;
    string      m_pathToCGI;
    string      m_extension;
    string      m_root;
    string      m_ip;
    string      m_port;
    string      m_body;
    string      m_filenameOut;
    string      m_filenameFrom;
    int         m_cgiOut;
    int         m_cgiReadFrom;
    char        **m_env;
    char        **m_args;

public:
    /**
     *
     * @param request - session request
     * @param pathToCGI - interpretator full path
     * @param extension - file extension
     * @param root - ?
     * @param ip - ?
     * @param port - ?
     */
    CGI(Request& request, const string& pathToCGI, const string& extension,
        const string& root, const string& ip, const string& port);
    ~CGI();
    string      execute();

private:
    void        createReadWriteFiles(int &read, int &write);
    char        **createEnv();
    char        **createArgs();

    void        destroyReadWriteFiles();
    void        destroyEnv();
    void        destroyArgs();

    void        writeBodyToFile();
    char        **convertMapToStrArray(std::map<string, string>& envMap);
    std::string gen_random(const int len);
    std::string getDataFileAsString(const std::string &filename);
};

#endif //WEBSERV_CGI_HPP
