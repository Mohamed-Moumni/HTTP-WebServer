#include "../INCLUDES/request.hpp"

int prefix_match(std::string s1, std::string s2)
{
    int minm = std::min(s1.size(), s2.size());
    int i = 0;

    while(i < minm && s1[i] == s2[i])
        i++;
    return i;
}

void append_root(ConnectSocket &socket,Server &server,location &location)
{
    //append root
    if(location._root.size())
        socket._request.request_target = '.' + location._root + socket._request.request_target;
    else if(server._root.size())
        socket._request.request_target =  '.' + server._root + socket._request.request_target;
}

void redirect(ConnectSocket &socket,location location, Server server, ConfigFile configfile)
{
    std::ostringstream response;
    std::string loca;

    loca = location._return;
    response << "HTTP/1.1 301 Moved Permanently\r\n";
    response << "Location: " << loca << CRLF << CRLF;
    socket._response.response_string = response.str();
}

int check_max_size(ConnectSocket socket, ConfigFile configfile, location location)
{
    (void)socket;
    (void)configfile;
    (void)location;
    return 1;
}

int find_server(ConnectSocket socket, ConfigFile configfile, Server & server)
{
    std::deque<Server> possible_servers;

    for(size_t i = 0; i < configfile._servers.size(); i++)
    {
        if((configfile._servers[i]._listen.count(socket.IpAdress) || configfile._servers[i]._listen.count("0.0.0.0"))
        && (configfile._servers[i]._listen[socket.IpAdress]).find(socket.Port) != (configfile._servers[i]._listen[socket.IpAdress]).end())
            possible_servers.push_back(configfile._servers[i]);
    }
    for(size_t i = 0; i < possible_servers.size(); i++)
    {
        if(std::find(possible_servers[i]._server_names.begin(), possible_servers[i]._server_names.end(), socket._request.headers_map["Host"]) 
        == possible_servers[i]._server_names.end())
            possible_servers.erase(possible_servers.begin() + i);
    }
    if(!possible_servers.size())
    {
        if(!configfile._servers.size())
            return 0;
        else
            server = configfile._servers[0];
    }
    else
        server = possible_servers[0];
    return 1;
}

int find_location(ConnectSocket socket, Server server, location &final_location)
{
    int max_match = 0;

    for(size_t i = 0; i < server._locations.size(); i++)
    {
        if(prefix_match(socket._request.request_target, server._locations[i].path) > max_match)
        {
            final_location = server._locations[i];
            max_match = prefix_match(socket._request.request_target, server._locations[i].path);
        }
    }
    return 1;
}

int respond(ConnectSocket &socket, ConfigFile configfile)
{
    Server server;
    location location;

    socket._response.response_string.clear();
    find_server(socket, configfile, server);
    find_location(socket, server, location);
    if(!check_max_size(socket, configfile, location))
        return 0;
    append_root(socket, server, location);
    if(location._return.size())
    {
        redirect(socket, location , server ,configfile);
        return 0;
    }
    //should be decommanted when switching to the exeternal makefile
    if(socket._request.request_target.find("..") != std::string::npos)
        return(socket._response.response_string = respond_error("403", configfile), 0);

    std::cout << "target : "<< socket._request.request_target << std::endl;
    response_generator(socket, server, location, configfile);
    // std::cout << socket._response.response_string << std::endl;
    // std::cout << "the chosen server is : " << server._server_names[0] << std::endl;
    // std::cout << "the chosen location is : " << location.path << std::endl;
    socket._response.respLength = socket._response.response_string.size();
    return 1;
}