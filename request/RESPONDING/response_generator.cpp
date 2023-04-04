#include "../INCLUDES/request.hpp"

int isallowed_method(ConnectSocket &socket, location &location, Server &server)
{
    if(location._allowed_methods.size() || server._allowed_methods.size())
    {
        if((location._allowed_methods.size() && !std::count(location._allowed_methods.begin(), location._allowed_methods.end(), socket._request.method))
        || (!location._allowed_methods.size() && !std::count(server._allowed_methods.begin(), server._allowed_methods.end(), socket._request.method)))
        {
            socket._response.response_string = respond_error("405");
            return 0;
        }
    }
    return 1;
}

void response_generator(ConnectSocket &socket, Server &server, location &location)
{
    if(!isallowed_method(socket, location, server))
        return ;
    if(socket._request.method == "GET")
        GET(socket, server, location);
    else if(socket._request.method == "POST")
        POST();
    else if(socket._request.method == "DELETE")
        DELETE();
    else
        socket._response.response_string = respond_error("501");
}