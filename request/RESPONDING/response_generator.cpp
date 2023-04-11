#include "../INCLUDES/request.hpp"

int isallowed_method(ConnectSocket &socket, location &location, Server &server, ConfigFile configfile)
{
    if(location._allowed_methods.size() || server._allowed_methods.size())
    {
        if((location._allowed_methods.size() && !std::count(location._allowed_methods.begin(), location._allowed_methods.end(), socket._request.method))
        || (!location._allowed_methods.size() && !std::count(server._allowed_methods.begin(), server._allowed_methods.end(), socket._request.method)))
        {
            socket._response.response_string = respond_error("405", configfile);
            return 0;
        }
    }
    return 1;
}

void response_generator(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile)
{
    if(!isallowed_method(socket, location, server, configfile))
        return ;
    if(socket._request.method == "GET")
        GET(socket, server, location, configfile);
    else if(socket._request.method == "POST")
        POST(socket, server, location, configfile);
    else if(socket._request.method == "DELETE")
        DELETE(socket, server, location, configfile);
    else
        socket._response.response_string = respond_error("501", configfile);
}