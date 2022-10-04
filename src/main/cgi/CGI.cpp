#include "CGI.hpp"

CGI::CGI(Request& request,
         const string& resource,
         const string& pathToCGI,
         const string& extension,
         const string& root,
         const string& ip,
         const string& port)
        :
        m_request(request),
        m_requiredResource(resource),
        m_pathToCGI(pathToCGI),
        m_extension(extension),
        m_root(root),
        m_ip(ip),
        m_port(port) {

    m_env = createEnv();

}

CGI::~CGI() {
    destroyArgs();
    destroyEnv();
    destroyReadWriteFiles();
}

string CGI::execute(const string& body) {
    createReadWriteFiles(m_cgiReadFrom, m_cgiOut);
    m_args = createArgs();
    writeBodyToFile(body);


    int pid = fork();
    if (pid < 0) {
        throw CGIException("Fork error.");
    }

    if (pid == 0) {
        dup2(m_cgiOut, 1);
        dup2(m_cgiReadFrom, 0);

        if (execve(m_args[0], m_args, m_env) == -1) {
            throw CGIException("Execve error.");
        }

        close(m_cgiOut);
        close(m_cgiReadFrom);
        exit(1);
    }

    string handledBody;
    if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        handledBody = getDataFileAsString(m_filenameOut);
        handledBody.erase(0, handledBody.find("\r\n\r\n") + 4);

        close(m_cgiOut);
        close(m_cgiReadFrom);
    }
    return handledBody;
}

void CGI::writeBodyToFile(const string& body) {
    write(m_cgiReadFrom, body.c_str(), body.length());
    lseek(m_cgiReadFrom, 0, SEEK_SET);
}

char** CGI::createEnv() {
    size_t pos = m_request.getUri().rfind('?');;
    std::map<string, string> envMap;
    envMap["REDIRECT_STATUS"] = "200"; //todo need to be approved
    envMap["GATEWAY_INTERFACE"] = "CGI/1.1";
    envMap["SERVER_NAME"] = m_ip;
    envMap["SERVER_PORT"] = m_port;
    envMap["SERVER_SOFTWARE"] = "webserv/1.0.0";
//    envMap["SERVER_PROTOCOL"] = m_request.findHeaderValue("protocol"); //todo could be "HTTP/1.1"
    envMap["SERVER_PROTOCOL"] = "HTTP/1.1";
    envMap["SCRIPT_NAME"] = m_pathToCGI;
    envMap["PATH_INFO"] = m_requiredResource;

    envMap["REQUEST_METHOD"] = m_request.getMethodString();

    envMap["DOCUMENT_URI"] = pos == std::string::npos ? m_request.getUri() : m_request.getUri().substr(0, pos);
    envMap["REQUEST_URI"] = m_request.getUri();
//    envMap["QUERY_STRING"] = ; //todo get query
    envMap["DOCUMENT_ROOT"] = m_root;

    envMap["PATH_TRANSLATED"] = m_pathToCGI;
    envMap["AUTH_TYPE"] = m_request.findHeaderValue("Authorization");
    envMap["REMOTE_USER"] = m_request.findHeaderValue("Authorization");
    envMap["REMOTE_IDENT"] = m_request.findHeaderValue("Authorization");
    envMap["CONTENT_TYPE"] = m_request.findHeaderValue("Сontent-type");
    envMap["CONTENT_LENGTH"] = m_request.findHeaderValue("Сontent-length");
    return convertMapToStrArray(envMap);
}

//todo make pathes relative!
char** CGI::createArgs() {
    char** args = new char* [2];
//    args[0] = strdup(("." + m_pathToCGI).c_str());
    args[0] = strdup((m_pathToCGI).c_str());
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

char** CGI::convertMapToStrArray(std::map<string, string>& envMap) {
    char** env = new char* [envMap.size() + 1];
    std::map<string, string>::iterator it = envMap.begin();
    std::map<string, string>::iterator end = envMap.end();
    int j = 0;
    for (; it != end; ++it) {
        string elem = it->first + "=" + it->second;
        env[j] = new char[elem.size() + 1];
        env[j] = strcpy(env[j], (const char*) elem.c_str());
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
    srand((unsigned) time(NULL) * getpid());
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

void CGI::createReadWriteFiles(int& read, int& write) {
    std::string uniqueHash = gen_random(8);
    m_filenameOut = "./cgi/cgi_out" + uniqueHash + ".txt";
    m_filenameFrom = "./cgi/cgi_read_from" + uniqueHash + ".txt";
    write = open(m_filenameOut.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
    read = open(m_filenameFrom.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
}

std::string CGI::getDataFileAsString(const std::string& filename) {
    std::stringstream buffer;
    std::ifstream file(filename);
    buffer << file.rdbuf();
    return buffer.str();
}
