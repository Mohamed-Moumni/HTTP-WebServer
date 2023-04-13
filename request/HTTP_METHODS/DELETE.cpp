#include "../INCLUDES/request.hpp"

void deletefile(ConnectSocket &socket)
{
    remove(socket._request.request_target.c_str());
    std::string body = "<h1 style=\"color: green\"> Target deleted succesfully!</h1>";
    std::ostringstream resp;
    resp << "HTTP/1.1 200 OK\r\nContent-Length: " << body.size() << "\r\nContent-Type: text/html\r\n\r\n" << body;
    socket._response.response_string = resp.str();
}

void DELETE(ConnectSocket &socket, Server server, location location, ConfigFile configfile)
{
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
        socket._response.response_string = respond_error("403", configfile);
    else
    {
        if(!access(socket._request.request_target.c_str(), F_OK) && !isdirectory(socket._request.request_target))
        {
            if(!access(socket._request.request_target.c_str(), W_OK))
                deletefile(socket);
            else
                socket._response.response_string = respond_error("403", configfile);
                
        }
        else
            socket._response.response_string = respond_error("404", configfile);
    }
}