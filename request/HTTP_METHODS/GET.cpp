#include "../INCLUDES/request.hpp"
#include <dirent.h>

void file2response(ConnectSocket &socket, Server &server, location &location)
{
    (void)server;
    (void)location;
    socket._response.response_string = read_file(socket._request.request_target);
}

int listdir(ConnectSocket &socket)
{
    DIR *dir;
    struct dirent *ent;
    std::string resp;

    dir = opendir(socket._request.request_target.c_str());
    if(!dir)
        return 0;
    while((ent = readdir(dir)))
    {    
        resp += ent->d_name;
        resp += '\n';
    }
    std::cout << resp <<  std::endl;
    socket._response.response_string = resp;
    closedir(dir);
    return 1;
}

void GET(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile)
{
    (void)configfile;
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(location._index.size())
            socket._request.request_target += location._index[0];
        else if(server._index.size())
            socket._request.request_target += server._index[0];
    }
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/' && (location._autoindex == "on"
    || (!location._autoindex.size() && server._autoindex == "on")))
    {
    std::cout << socket._request.request_target << std::endl;
        if(!listdir(socket))
            socket._response.response_string = respond_error("404");
        return ;
    }
    else if (socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(opendir(socket._request.request_target.c_str()) != NULL)
            socket._response.response_string = respond_error("403");//responde with 403 forbidden and 
        else
            socket._response.response_string = respond_error("404");//responde with 404 notfound and
        return ;
    }
    //check if the content is dynamic or static and server each one separatly
    if(socket._request.request_target.size() >= 4 && !strcmp(socket._request.request_target.c_str() + socket._request.request_target.size() - 4, ".php"))
    {
        // std::cout << "dynamic content detected" << std::endl;
        socket._response.response_string = respond_error("CGI not working yet");
        return;
    }
    
    //static here
    std::cout << socket._request.request_target << std::endl;
    if(!access(socket._request.request_target.c_str(), F_OK))
        file2response(socket, server, location);
    else 
        socket._response.response_string = respond_error("404"); //404 not found 
}