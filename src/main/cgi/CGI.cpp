#include "CGI.hpp"

CGI::CGI(Request& request, const string& pathToCGI, const string& extension,
    const string& root, const string& ip, const string& port)
: m_request(request), m_pathToCGI(pathToCGI), m_extension(extension), m_root(root), m_ip(ip), m_port(port) {
    createReadWriteFiles(m_cgiReadFrom, m_cgiOut);
    m_env = createEnv();
    m_args = createArgs();
}

CGI::~CGI() {
    destroyArgs();
    destroyEnv();
    destroyReadWriteFiles();
}

string CGI::execute() {
    writeBodyToFile();
    int pid;
    int status;
    if ((pid = fork()) == -1) {
        throw CGIException("Fork error.");
    }
    else if (pid == 0)
    {
        dup2(m_cgiOut, 1);
        dup2(m_cgiReadFrom, 0);
        if (execve(m_args[0], m_args, m_env) == -1) {
            throw CGIException("Execve error.");
        }
        close(m_cgiOut);
        close(m_cgiReadFrom);
        exit(1);
    }
    else if (pid > 0) {
        waitpid(pid, &status, 0);
        m_body = getDataFileAsString(m_filenameOut);
        m_body.erase(0, m_body.find("\r\n\r\n") + 4);
        close(m_cgiOut);
        close(m_cgiReadFrom);
    }
    return m_body;
}

//todo
void CGI::writeBodyToFile() {
    write(m_cgiReadFrom, m_request.getBody().c_str(), m_request.getBody().length());
    lseek(m_cgiReadFrom, 0, SEEK_SET);
}

//todo
//char **CGI::createEnv() {
//    std::map<string, string> envMap;
//    //CONTENT_LENGTH
//    envMap["GATEWAY_INTERFACE"] = m_request.findHeaderValue("content-length");
//    //CONTENT_TYPE
//    envMap["CONTENT_TYPE"] = m_request.findHeaderValue("content-type");
//    //GATEWAY_INTERFACE
//    envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
//    //PATH_INFO - request uri
//    envMap["PATH_INFO"] = m_request.getUri();
//    //REQUEST_METHOD
//    envMap["REQUEST_METHOD"] = m_request.findHeaderValue("method");
//    //SERVER_PROTOCOL
//
//    //DOCUMENT_URI
//    //REQUEST_URI
//    //QUERY_STRING
//    //SCRIPT_NAME
//    //SCRIPT_FILENAME
//    envMap["SCRIPT_FILENAME"] = m_root + m_pathToCGI;
//    //DOCUMENT_ROOT
//    envMap["DOCUMENT_ROOT"] = m_root;
//    //SERVER_NAME
//    envMap["SERVER_NAME"] = m_ip;
//    //SERVER_PORT
//    envMap["SERVER_PORT"] = m_port;
//    //SERVER_SOFTWARE
//    envMap["SERVER_SOFTWARE"] = "http_server/1.0.0";
//    return convertMapToStrArray(envMap);
//}

char **CGI::createEnv() {
    std::map<string, string> envMap;
    envMap["REDIRECT_STATUS"] = "200"; //todo need to be approved
    envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
    envMap["SERVER_NAME"] = m_ip;
    envMap["SERVER_SOFTWARE"] = "webserv";
    envMap["SERVER_PROTOCOL"] = m_request.findHeaderValue("protocol"); //todo could be "HTTP/1.1"
    envMap["SERVER_PORT"] = m_port;
    envMap["REQUEST_METHOD"] = m_request.getMethodString();
    envMap["PATH_INFO"] = m_pathToCGI;
    envMap["PATH_TRANSLATED"] = m_pathToCGI;
    envMap["SCRIPT_NAME"] = m_pathToCGI;
    envMap["DOCUMENT_ROOT"] = m_root;
//    envMap["QUERY_STRING"] = ; //todo get query
    envMap["AUTH_TYPE"] = m_request.findHeaderValue("Authorization");
    envMap["REMOTE_USER"] = m_request.findHeaderValue("Authorization");
    envMap["REMOTE_IDENT"] = m_request.findHeaderValue("Authorization");
    envMap["CONTENT_TYPE"] = m_request.findHeaderValue("content-type");
    envMap["CONTENT_LENGTH"] = m_request.findHeaderValue("content-length");
    return convertMapToStrArray(envMap);
}

char **CGI::createArgs()
{
    char **args = new char*[2];
    args[0] = strdup(("." + m_pathToCGI).c_str());
    args[1] = NULL;
    return args;
}

void CGI::destroyReadWriteFiles() {
    std::remove(m_filenameOut.c_str());
    std::remove(m_filenameFrom.c_str());
}

void CGI::destroyEnv() {
    if (m_env) {
        for (int i = 0; m_env[i] != NULL; i++) {
            delete m_env[i];
        }
        delete[] m_env;
    }
}

void CGI::destroyArgs() {
    delete m_args[0];
    delete[] m_args;
}

char **CGI::convertMapToStrArray(std::map<string, string>& envMap) {
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

std::string CGI::gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    srand( (unsigned) time(NULL) * getpid());
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

void CGI::createReadWriteFiles(int &read, int &write) {
    std::string uniqueHash = gen_random(8);
    m_filenameOut = "./cgi/cgi_out" + uniqueHash;
    m_filenameFrom = "./cgi/cgi_read_from" + uniqueHash;
    write = open(m_filenameOut.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
    read = open(m_filenameFrom.c_str() , O_RDWR | O_TRUNC | O_CREAT, 0777);
}

std::string CGI::getDataFileAsString(const std::string &filename) {
    std::stringstream buffer;
    std::ifstream file(filename);
    buffer << file.rdbuf();
    return buffer.str();
}
