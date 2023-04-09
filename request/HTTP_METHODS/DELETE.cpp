#include "../INCLUDES/request.hpp"

void deletefile(ConnectSocket &socket)
{
    remove(socket._request.request_target.c_str());
    socket._response.response_string = "target deleted";
}

void DELETE(ConnectSocket &socket, Server server, location location, ConfigFile configfile)
{
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
        socket._response.response_string = respond_error("403", configfile);
    else
    {
        std::cout << socket._request.request_target << std::endl;
        if(!access(socket._request.request_target.c_str(), F_OK))
            deletefile(socket);
        else
            socket._response.response_string = respond_error("404", configfile);
    }
}