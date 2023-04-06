#include "../INCLUDES/request.hpp"
#include <dirent.h>
#include <fstream>



void createfile(ConnectSocket &socket)
{
    //check the content type in the mimetype map
    std::ofstream targeted_file(socket._request.request_target);
    targeted_file << socket._request.request_body;
    targeted_file.close();
    //todo
    socket._response.response_string = "201 created";
}

void POST(ConnectSocket &socket, Server server, location location)
{
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
        socket._response.response_string = respond_error("403");
    else
    {
        std::cout << socket._request.request_target << std::endl;
        if(!access(socket._request.request_target.c_str(), F_OK))
            socket._response.response_string = respond_error("409");
        else
        {
            if(opendir(socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/')).c_str()))
                createfile(socket);
            else
                socket._response.response_string = respond_error("404");
        }
    }
}