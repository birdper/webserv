#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP
#include <map>
#include <string>

class CGI {
private:
public:
    CGI() {}
    ~CGI() {}
    void execute() {
        std::map<std::string, std::string> env_map;
        env_map["REDIRECT_STATUS"] = "200";
//        env_map["GATEWAY_INTERFACE"] = "GATEWAY_INTERFACE";
//        env_map["SERVER_PROTOCOL"] = "SERVER_PROTOCOL";
//        env_map["SERVER_SOFTWARE"] = "SERVER_SOFTWARE";
//        env_map["REQUEST_METHOD"] = "REQUEST_METHOD";
//        env_map["REQUEST_URI"] = "REQUEST_URI";
//        env_map["PATH_INFO"] = "PATH_INFO";
//        env_map["PATH_TRANSLATED"] = "PATH_TRANSLATED";
//        env_map["QUERY_STRING"] = "QUERY_STRING";
//        env_map[""] = "";

        char **env_arg = convert_map_to_str_array(env_map);

    }

private:
    char **convert_map_to_str_array(std::map<std::string, std::string>& env_map) {
        char	**env = new char*[env_map.size() + 1];

        std::map<std::string, std::string>::iterator it = env_map.begin();
        std::map<std::string, std::string>::iterator end = env_map.end();
        int  j = 0;
        for (; it != end; ++it) {
            std::string elem = it->first + "=" + it->second;
            env[j] = new char[elem.size() + 1];
            env[j] = strcpy(env[j], (const char*)elem.c_str());
            j++;
        }
        env[j] = NULL;
        return env;
    }
};

#endif //WEBSERV_CGI_HPP
