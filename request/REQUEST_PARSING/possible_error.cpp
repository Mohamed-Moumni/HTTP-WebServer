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
    if(!get_contentL(socket, configfile))
        return 0;
    if(socket._request.http_version != "HTTP/1.1")
        return 0;
    if(socket._request.method == "GET" && socket._request.request_body.size())
        return 0;
    if(socket._request.method == "POST" && socket._request.headers_map.find("Content-Type") == socket._request.headers_map.end())
    {
        socket._response.response_string = respond_error("400", configfile);
        socket._response.respLength = socket._response.response_string.size();
        return 2;
    }

    if(socket._request.method == "POST" && socket._request.headers_map.find("Content-Length") == socket._request.headers_map.end())
    {
        socket._response.response_string = respond_error("411", configfile);
        socket._response.respLength = socket._response.response_string.size();
        return 2;
    }
    if(socket._request.request_target.size() > 10000)
    {
        socket._response.response_string = respond_error("414", configfile);
        socket._response.respLength = socket._response.response_string.size();        
        return 2;
    }
    return 1;
}
