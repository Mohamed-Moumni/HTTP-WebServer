#include "../INCLUDES/request.hpp"
#include <dirent.h>
#include <fstream>

void createfile(ConnectSocket &socket, ConfigFile configfile, location location)
{
    std::ofstream targeted_file;
    std::string filepath;

    filepath = socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/'));
    if(location._upload.size())
        filepath += '/' +location._upload;
    filepath += socket._request.request_target.substr(socket._request.request_target.find_last_of('/'));

    if(configfile._content_types.find(socket._request.headers_map["Content-Type"]) == configfile._content_types.end())
    {
        socket._response.response_string = respond_error("415", configfile);
        return;
    }

    targeted_file.open(filepath);
    if(!targeted_file)
    {
        socket._response.response_string = respond_error("403", configfile);
        return ;
    }
    targeted_file << socket._request.request_body;
    targeted_file.close();
    socket._response.response_string = "HTTP/1.1 201 Created\r\nLocation: " + filepath;
    socket._response.response_string += "\n\rContent-Length: 29\r\n\r\n<h1>Created successfully!<h1>";
}

void POST(ConnectSocket &socket, Server server, location location, ConfigFile configfile)
{
    DIR * dir;

    std::cout << "enter to post " << std::endl;
    if(socket._request.request_target.find('?') != std::string::npos)
    {
        socket._response.response_string = respond_error("400", configfile);
        return ;
    }

    //remove the path_info from the uri
    if(socket._request.request_target.find(".php") != std::string::npos)
        socket._request.request_target = socket._request.request_target.substr(0, socket._request.request_target.find(".php") + 4);

    //////////////////////check if post for directory
    if((dir = opendir(socket._request.request_target.c_str())))
    {
        closedir(dir);
        socket._response.response_string = respond_error("403", configfile);
        return;
    }
    else if(socket._request.request_target[socket._request.request_target.size() - 1] == '/') ////if it's file and slash in the end the slash should be removed
    {
        socket._request.request_target[socket._request.request_target.size() - 1] = 0;
    }

    /////////////////////////Dynamic here
    if(!access(socket._request.request_target.c_str(), F_OK))
    {
        if(socket._request.request_target.size() >= 4 && get_extention(socket._request.request_target) == ".php")
        {
            cgi_handler(socket, location, server, configfile);
            return ;
        }
    }

    //////////////////////Static here
    if(!access((socket._request.request_target + location._upload).c_str(), F_OK))
    {
        socket._response.response_string = respond_error("409", configfile);
    }
    else
    {
        if((!access((socket._request.request_target.substr(0, socket._request.request_target.find_last_of('/')) + '/' + location._upload).c_str(), F_OK)))
            createfile(socket, configfile, location);
        else
            socket._response.response_string = respond_error("404", configfile);
    }

    std::cout << "end of post " << std::endl;
}