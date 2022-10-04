
#pragma once

#include <map>
#include <string>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

#include "../exception/CGIException.hpp"
#include "Request.hpp"
#include "usings.hpp"


//Reference https://www.oreilly.com/openbook/cgi/ch02_02.html
//Table 2.1: List of CGI Environment Variables
//        Environment Variable
//
//        Description
//
//GATEWAY_INTERFACE
//
//        The revision of the Common Gateway Interface that the server uses.
//
//SERVER_NAME
//
//        The server's hostname or IP address.
//
//SERVER_SOFTWARE
//
//        The name and version of the server software that is answering the client request.
//
//SERVER_PROTOCOL
//
//        The name and revision of the information protocol the request came in with.
//
//SERVER_PORT
//
//        The port number of the host on which the server is running.
//
//REQUEST_METHOD
//
//        The method with which the information request was issued.
//
//PATH_INFO
//
//        Extra path information passed to a CGI program.
//
//PATH_TRANSLATED
//
//        The translated version of the path given by the variable PATH_INFO.
//
//SCRIPT_NAME
//
//        The virtual path (e.g., /cgi-bin/program.pl) of the script being executed.
//
//DOCUMENT_ROOT
//
//        The directory from which Web documents are served.
//
//QUERY_STRING
//
//        The query information passed to the program. It is appended to the URL with a "?".
//
//REMOTE_HOST
//
//        The remote hostname of the user making the request.
//
//REMOTE_ADDR
//
//        The remote IP address of the user making the request.
//
//AUTH_TYPE
//
//        The authentication method used to validate a user.
//
//REMOTE_USER
//
//        The authenticated name of the user.
//
//REMOTE_IDENT
//
//        The user making the request. This variable will only be set if NCSA IdentityCheck flag is enabled, and the client machine supports the RFC 931 identification scheme (ident daemon).
//
//CONTENT_TYPE
//
//        The MIME type of the query data, such as "text/html".
//
//CONTENT_LENGTH
//
//        The length of the data (in bytes or the number of characters) passed to the CGI program through standard input.
//
//HTTP_FROM
//
//        The email address of the user making the request. Most browsers do not support this variable.
//
//HTTP_ACCEPT
//
//        A list of the MIME types that the client can accept.
//
//HTTP_USER_AGENT
//
//        The browser the client is using to issue the request.
//
//HTTP_REFERER
//
//        The URL of the document that the client points to before accessing the CGI program.
//
class CGI {
private:
    Request&    m_request;
    string      m_requiredResource;
    string      m_pathToCGI;
    string      m_extension;
    string      m_root;
    string      m_ip;
    string      m_port;
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
     * @param root - root
     * @param ip - ?
     * @param port - ?
     */
    CGI(Request& request,
        const string& resource,
        const string& pathToCGI,
        const string& extension,
        const string& root,
        const string& ip,
        const string& port);
    ~CGI();
    string execute(const string& body);

private:
    void        createReadWriteFiles(int &read, int &write);
    char        **createEnv();
    char        **createArgs();

    void        destroyReadWriteFiles();
    void        destroyEnv();
    void        destroyArgs();

    void writeBodyToFile(const string& body);
    char        **convertMapToStrArray(std::map<string, string>& envMap);
    std::string gen_random(const int len);
    std::string getDataFileAsString(const std::string &filename);
};
