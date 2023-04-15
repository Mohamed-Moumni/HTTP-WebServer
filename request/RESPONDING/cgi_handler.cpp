#include "../INCLUDES/request.hpp"
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>

std::string conc(std::string s1, std::string s2)
{
    return s1 + '=' + s2;
}

static std::string  generateToken(int length)
{
    std::string token = "";
    static const char charset[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(nullptr));
    for (int i = 0; i < length; i++)
    {
        token += charset[rand() % (sizeof(charset) - 1)];
    }
    return token;
}

static std::string  readFromPipe(int PipeId)
{
    int             readedChar;
    std::string     data;
    char            buffer[BUFFER] = {0};

    readedChar = read(PipeId, buffer, BUFFER);
    while (readedChar > 0)
    {
        data += buffer;
        readedChar = read(PipeId, buffer, BUFFER);
    }
    return (data);
}

void set_env(ConnectSocket &socket, location location,Server server, ConfigFile configfile, char **env)
{
    //setting SERVER_NAME
    std::string server_name = socket.IpAdress + ":" + socket.Port;

    //setting PATH_INFO
    std::string path_info = socket._request.original_request_target.substr(socket._request.original_request_target.find(".php") + 4,\
    (socket._request.original_request_target.find('?') ) - (socket._request.original_request_target.find(".php") + 4));

    //setting SCRIPTNAME
    std::string script_name = socket._request.request_target.substr(socket._request.request_target.find_last_of('/')\
    + 1, socket._request.request_target.find(".php") + 4);

    //setting QUERYSTRING
    std::string querystring;
    if(socket._request.method == "GET")
    {
        if(socket._request.original_request_target.find('?') != std::string::npos)
            querystring = socket._request.original_request_target.substr(socket._request.original_request_target.find('?') + 1);
    }
    else
        querystring = socket._request.request_body;

    env[0] = strdup("SERVER_SOFTWARE=NGINY/0.1");
    env[1] = strdup(conc("SERVER_NAME", server_name).c_str());
    env[2] = strdup(conc("SERVER_PROTOCOL", "HTTP/1.1").c_str());
    env[3] = strdup(conc("SERVER_PORT", socket.Port).c_str());
    env[4] = strdup(conc("REQUEST_METHOD", socket._request.method).c_str());
    env[5] = strdup(conc("PATH_INFO", path_info).c_str());
    env[6] = strdup(conc("SCRIPT_FILENAME", socket._request.request_target).c_str());
    env[7] = strdup(conc("QUERY_STRING", querystring).c_str());
    env[8] = strdup(conc("REDIRECT_STATUS", "301").c_str());
    if(socket._request.method == "POST")
    {
        env[9] = strdup(conc("CONTENT_TYPE", socket._request.headers_map["Content-type"]).c_str());
        env[10] = strdup(conc("CONTENT_LENGTH", socket._request.headers_map["Content-Length"]).c_str());
    }
}

void cgi_handler(ConnectSocket &socket, location location,Server server, ConfigFile configfile)
{
    int         pid;
    int         fds[2];
    std::string body;
    int         tmpfile;
    long long timeOut;

    if (pipe(fds) < 0)
    {
        std::cout << "PIPE Error\n";
        return ;
    }
    tmpfile = open("/tmp/tempfd", O_CREAT | O_RDWR , 0644);
    write(tmpfile, socket._request.request_body.c_str(), socket._request.request_body.size());
    lseek(tmpfile, 0, SEEK_SET);
    timeOut = getTimeOfNow();
    pid = fork();
    if (pid == 0)
    {
        char *env[12];
        char *args[3];

        set_env(socket, location, server, configfile, env);
        std::cout << "querystring: " << env[7] << std::endl;
        if(socket._request.method == "POST")
            env[11] = NULL;
        else
            env[9] = NULL;
        args[0] = strdup("./cgi-bin/php-cgi");
        args[1] = strdup(socket._request.request_target.c_str());
        args[2] = NULL;
        dup2(tmpfile, STDIN_FILENO);
        close(tmpfile);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        if(execve(args[0], args, env) == -1)
            std::cout << errno << std::endl;
    }
    long long rest = getTimeOfNow() - timeOut;
    while (rest < 5)
    {
        if (waitpid(pid, NULL, WNOHANG) == pid)
        {
            close(fds[1]);
            socket._response.response_string = readFromPipe(fds[0]);
            std::cout << "body: +++++++++++++++++\n" << body << "+++++++++++++++++++++\n";
            close(fds[0]);
            std::ostringstream out;
            out << "HTTP/1.1 200 OK\r\nSet-Cookie: NginY=" << generateToken(10) << ";Max-Age=30"<< "\r\nContent-Length: " << socket._response.response_string.substr(socket._response.response_string.find("\r\n\r\n") + 4).size() << "\r\n" << socket._response.response_string;
            socket._response.response_string = out.str();
            std::cout << socket._response.response_string << std::endl;
            unlink("/tmp/tempfd");
            return ;
        }
        else
            rest = getTimeOfNow() - timeOut;
    }
    kill(pid, SIGKILL);
    close(fds[1]);
    close(fds[0]);
    unlink("/tmp/tempfd");
    socket._response.response_string = respond_error("504", configfile);
    socket.closed = true;
}