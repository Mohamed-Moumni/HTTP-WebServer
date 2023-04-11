#include "../INCLUDES/request.hpp"
#include <dirent.h>
#include <fstream>

void createfile(ConnectSocket &socket, ConfigFile configfile)
{
    std::ofstream targeted_file;

    if(configfile._content_types.find(socket._request.headers_map["Content-Type"]) == configfile._content_types.end())
    {
        socket._response.response_string = respond_error("415", configfile);
        return;
    }
    
    targeted_file.open(socket._request.request_target);
    if(targeted_file.failbit)
    {
        socket._response.response_string = respond_error("403", configfile);
        return ;
    }
    targeted_file << socket._request.request_body;
    targeted_file.close();
    socket._response.response_string = "HTTP/1.1 201 Created\r\nLocation: " + socket._request.request_target + "\r\n\r\n";
}

void POST(ConnectSocket &socket, Server server, location location, ConfigFile configfile)
{
    DIR * dir;

    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
        socket._response.response_string = respond_error("403", configfile);
    else
    {
        std::cout << socket._request.request_target << std::endl;
        // std::cout << "check for : " << socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/')) << std::endl;
        if(!access(socket._request.request_target.c_str(), F_OK))
            socket._response.response_string = respond_error("409", configfile);
        else
        {
            std::cout << "checking for : "<< socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/')).c_str() << std::endl;
            if((dir = opendir(socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/')).c_str())))
            {
                closedir(dir);
                createfile(socket, configfile);
            }
            else
                socket._response.response_string = respond_error("404", configfile);
        }

    }
}