#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP
#include <map>
#include <string>
#include "Request.hpp"
#include "usings.hpp"

class CGI {
private:
    string m_pathToCGI;
    string m_extension;
    string m_root;
    string m_ip;
    string m_port;

public:
    CGI(const string& pathToCGI, const string& extension,
        const string& root, const string& ip, const string& port);

    ~CGI();

    void execute(Request& request) {
        char **env = getEnv(request);

    }

private:

    char **getEnv(Request& request) {
        std::map<string, string> envMap;
        envMap["REDIRECT_STATUS"] = "200";
        envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
        envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
        envMap["SERVER_SOFTWARE"] = "webserv/1.0";
        envMap["REQUEST_METHOD"] = request.getMethodString();
        envMap["REQUEST_URI"] = request.getUri();
        envMap["PATH_INFO"] = getPathInfo(path);
        envMap["PATH_TRANSLATED"] = getPathInfo(path);
        envMap["QUERY_STRING"] = query;
        string header = request.findHeaderValue("Authorization");
        if (!header.empty()) {
            envMap["AUTH_TYPE"] = header;
            envMap["REMOTE_IDENT"] = header;
            envMap["REMOTE_USER"] = header;
        }
        header = request.findHeaderValue("Content-Type");
        if (!header.empty()) {
            envMap["CONTENT_TYPE"] = header;
        }
        envMap["SERVER_NAME"] = request.findHeaderValue("Host");
        return convertMapToStrArray(envMap);
    }

    char **convertMapToStrArray(std::map<string, string>& envMap) {
        char	**env = new char*[envMap.size() + 1];

        std::map<string, string>::iterator it = envMap.begin();
        std::map<string, string>::iterator end = envMap.end();
        int  j = 0;
        for (; it != end; ++it) {
            string elem = it->first + "=" + it->second;
            env[j] = new char[elem.size() + 1];
            env[j] = strcpy(env[j], (const char*)elem.c_str());
            j++;
        }
        env[j] = NULL;
        return env;
    }
};

#endif //WEBSERV_CGI_HPP
