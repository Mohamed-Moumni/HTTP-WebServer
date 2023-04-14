#include "../INCLUDES/request.hpp"


std::string conc(std::string s1, std::string s2)
{
    return s1 + '=' + s2;
}


void set_env(ConnectSocket &socket, location location,Server server, ConfigFile configfile, char **env)
{
    std::string server_name = socket.IpAdress + ":" + socket.Port;
    std::string path_info = socket._request.request_target.substr(socket._request.request_target.find(".php"), socket._request.request_target.find('?'));
    std::string script_name = socket._request.request_target.substr(socket._request.request_target.find_last_of('/'), socket._request.request_target.find(".php") + 4);
    std::string querystring = socket._request.request_target.substr(socket._request.request_target.find('?'));
    env[0] = strdup("SERVER_SOFTWARE=NGINY/0.1");
    env[1] = strdup(conc("SERVER_NAME", server_name).c_str());
    env[2] = strdup(conc("SERVER_PROTOCOL", "HTTP/1.1").c_str());
    env[3] = strdup(conc("SERVER_PORT", socket.Port).c_str());
    env[4] = strdup(conc("REQUEST_METHOD", socket._request.method).c_str());
    if(socket._request.method == "POST")
    {
        env[5] = strdup(conc("CONTENT_TYPE", socket._request.headers_map["Content-type"]).c_str());
        env[6] = strdup(conc("CONTENT_LENGTH", socket._request.headers_map["Content-Length"]).c_str());
    }
    env[7] = strdup(conc("PATH_INFO", path_info).c_str());
    env[8] = strdup(conc("SCRIPT_NAME", script_name).c_str());
    env[9] = strdup(conc("QUERY_STRING", querystring).c_str());
}

void cgi_handler(ConnectSocket &socket, location location,Server server, ConfigFile configfile)
{
    char *env[10];
    set_env(socket, location, ) 
    //setting env variables
    //fork.....
}