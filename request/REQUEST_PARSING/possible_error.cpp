#include "../INCLUDES/request.hpp"

int is_alldigit(std::string str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int get_contentL(ConnectSocket &socket, ConfigFile configfile)
{
    std::string map_value;
    (void)configfile;

    if(socket._request.headers_map.find("Content-Length") != socket._request.headers_map.end())
    {
        if(!is_alldigit(socket._request.headers_map["Content-Length"]))
            return 0;
        if(socket._request.headers_map["Content-Length"].size() < 10 || (socket._request.headers_map["Content-Length"].size() == 10 && socket._request.headers_map["Content-Length"] <= "2147483647"))
        {
            socket._request.ContentLen = stoi(socket._request.headers_map["Content-Length"]);
            if(socket._request.method != "POST" && socket._request.ContentLen && socket._request.headers_map["Transfer-Encoding"] != "chunked")
                return 0;
            // if(socket._request.ContentLen > configfile.)
        }
        else
            return 0;
    }
    else
        socket._request.ContentLen = 0;
    return 1;
}

int possible_error(ConnectSocket &socket, ConfigFile configfile)
{
    //todo
    // (void)socket;
    std::cout << "http version: " << socket._request.http_version << std::endl;
    if(!get_contentL(socket, configfile))
        return 0;
    if(socket._request.http_version != "HTTP/1.1")
        return 0;
    if(socket._request.method == "GET" && socket._request.request_body.size())
        return 0;
    return 1;
}
