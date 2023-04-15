#include "../INCLUDES/request.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <fstream>

std::string                 get_extention(std::string str)
{
    return str.substr(str.find_last_of('.'));
}

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

    std::cout << "here" << std::endl;
    file.open(file_name, std::ios::binary);
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
    DIR *dir;
    ///////////////////////////////////REMOVE PATH INFO IN CASE OF DYNAMIC
    if(socket._request.request_target.find(".php") != std::string::npos)
        socket._request.request_target = socket._request.request_target.substr(0, socket._request.request_target.find(".php") + 4);
    ///////////////////////////////////CHECK for slash at the end of files and dirs
    // std::cout << "before : " << socket._request.request_target << std::endl;
    if((dir = opendir(socket._request.request_target.c_str())))
    {
        if (socket._request.request_target[socket._request.request_target.size() - 1] != '/')
            socket._request.request_target += '/';
        closedir(dir);
    }
    else if (socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        socket._request.request_target[socket._request.request_target.size() - 1] = 0;
    }
    // std::cout << "after : " << socket._request.request_target << std::endl;
    /////////////////////////////////////add index directive
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        if(location._index.size())
            socket._request.request_target += location._index[0];
        else if(server._index.size())
            socket._request.request_target += server._index[0];
    }
    /////////////////////////////////////// check for directory listing
    if(socket._request.request_target[socket._request.request_target.size() - 1] == '/' && (location._autoindex == "on"
    || (!location._autoindex.size() && server._autoindex == "on")))
    {
        if(!listdir(socket))
            socket._response.response_string = respond_error("404", configfile);
        return ;
    }
    else if(socket._request.request_target[socket._request.request_target.size() - 1] == '/')
    {
        socket._response.response_string = respond_error("403", configfile);
        return;
    }
    //////////////////////////////////////check for dynamic or static content
    //dynamic here
    if(socket._request.request_target.size() >= 4 && (get_extention(socket._request.request_target) == ".php"))
    {
        cgi_handler(socket, location, server, configfile);
        return;
    }
    
    //static here
    if(!access(socket._request.request_target.c_str(), F_OK))
    {
        if(!access(socket._request.request_target.c_str(), R_OK))
            file2response(socket, server, location, configfile);
        else
            socket._response.response_string = respond_error("403", configfile);
    }
    else 
        socket._response.response_string = respond_error("404", configfile);
}