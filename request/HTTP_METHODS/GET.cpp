#include "../INCLUDES/request.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <fstream>

std::string get_contenttype(std::string file, ConfigFile configfile)
{
    int i = file.size();
    while(i > -1)
    {
        if(file[i] == '.')
            break;
        if(file[i] == '/')
            return "plain/text";
        i--;
    }
    return configfile._mime_types[file.c_str() + i];

}
std::string		GET_file(std::string file_name)
{
    std::ifstream file;
    std::ostringstream data;

    file.open(file_name);
    data << file.rdbuf();
    return data.str();
}

void file2response(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile)
{
    (void)server;
    (void)location;
    std::ostringstream response;
    socket._response.response_string = GET_file(socket._request.request_target);

    response  << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " + get_contenttype(socket._request.request_target, configfile) << CRLF ;
    response << "Content-Length: " << socket._response.response_string.size() << CRLF << CRLF;
    response << socket._response.response_string;
    socket._response.response_string = response.str();
}
int isdirectory(std::string path)
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir(path.c_str());
    if(dir)
    {
        closedir(dir);
        return 1;
    }
    return 0;
}

int listdir(ConnectSocket &socket)
{
    DIR *dir;
    struct dirent *ent;
    std::ostringstream resp;

    dir = opendir(socket._request.request_target.c_str());
    if(!dir)
        return 0;
    while((ent = readdir(dir)))
    {    
        socket._response.response_string += "<a href=\"";
        socket._response.response_string += ent->d_name;
        if(isdirectory(socket._request.request_target + ent->d_name))
            socket._response.response_string += "/";
        socket._response.response_string += "\">";
        socket._response.response_string += ent->d_name;
        if(isdirectory(socket._request.request_target + ent->d_name))
            socket._response.response_string += "/";
        socket._response.response_string += "</a><br>\n";
    }
    resp << "HTTP/1.1 200 OK\r\n";
    resp << "Content-Type: text/html" << CRLF ;
    resp << "Content-Length: " << socket._response.response_string.size() << CRLF << CRLF;
    resp << socket._response.response_string;
    socket._response.response_string = resp.str();
    closedir(dir);

    return 1;
}

void GET(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile)
{
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
        if(!listdir(socket))
            socket._response.response_string = respond_error("404", configfile);
        return ;
    }
    else if (socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(opendir(socket._request.request_target.c_str()) != NULL)
            socket._response.response_string = respond_error("403", configfile);//responde with 403 forbidden and 
        else
            socket._response.response_string = respond_error("404", configfile);//responde with 404 notfound and
        return ;
    }
    //check if the content is dynamic or static and server each one separatly
    if(socket._request.request_target.size() >= 4 && !strcmp(socket._request.request_target.c_str() 
    + socket._request.request_target.size() - 4, ".php"))
    {
        // std::cout << "dynamic content detected" << std::endl;
        socket._response.response_string = respond_error("CGI not working yet", configfile);
        return;
    }
    
    //static here
    if(!access(socket._request.request_target.c_str(), F_OK))
    {
        if(opendir(socket._request.request_target.c_str()))
            socket._response.response_string = respond_error("404", configfile);
        else if(!access(socket._request.request_target.c_str(), R_OK))
            file2response(socket, server, location, configfile);
        else
            socket._response.response_string = respond_error("403", configfile);

    }
    else 
        socket._response.response_string = respond_error("404", configfile); //404 not found 
}