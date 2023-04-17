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
    (void)server;
    (void)configfile;

    loca = location._return;
    // std::cout << loca << std::endl;
    response << "HTTP/1.1 301 Moved Permanently\r\n";
    response << "Location: " << loca << CRLF << CRLF;
    socket._response.response_string = response.str();
}

int check_max_size(ConnectSocket & socket, ConfigFile configfile, Server server)
{
    // std::cout << server._client_max_body_size << "vs" <<  socket._request.headers_map["Content-Length"] << std::endl;
    if(socket._request.request_body.size() > server._client_max_body_size)
    {

        socket._response.response_string = respond_error("413", configfile);
        return 0;
    }
    return 1;
}

int find_server(ConnectSocket socket, ConfigFile configfile, Server & server)
{
    std::vector<Server> possible_servers;
    std::vector<Server> removed_servers;


    for(size_t i = 0; i < configfile._servers.size(); i++)
    {
        if((configfile._servers[i]._listen.count(socket.IpAdress) || configfile._servers[i]._listen.count("0.0.0.0"))
        && (configfile._servers[i]._listen[socket.IpAdress]).find(socket.Port) != (configfile._servers[i]._listen[socket.IpAdress]).end())
            possible_servers.push_back(configfile._servers[i]);
    }
    // std::cout << "size = " << possible_servers.size() << std::endl;
    for(size_t i = 0; i < possible_servers.size(); i++)
    {
        // std::cout << i << " : "<< possible_servers[i]._server_names[0] << std::endl;
        if(std::find(possible_servers[i]._server_names.begin(), possible_servers[i]._server_names.end(), socket._request.headers_map["Host"]) 
        == possible_servers[i]._server_names.end())
        {
            removed_servers.push_back(possible_servers[i]);
            possible_servers.erase(possible_servers.begin() + i);
            i--;
        }
    }
    // std::cout << possible_servers[0]._server_names[0] << std::endl;
    if(!possible_servers.size())
    {
        // std::cout << "here\n" << std::endl;
        if(!configfile._servers.size())
            return 0;
        else
            server = removed_servers[0];
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
    static int rn;

    // printf("%d\n" , rn);
    rn ++;
    socket._response.response_string.clear();
    find_server(socket, configfile, server);
    find_location(socket, server, location);
    if(!check_max_size(socket, configfile, server))
        return 0;
    append_root(socket, server, location);
    if(location._return.size())
    {
        redirect(socket, location , server ,configfile);
        return 0;
    }
    if(socket._request.request_target.find("..") != std::string::npos)
        return(socket._response.response_string = respond_error("403", configfile), 0);
    response_generator(socket, server, location, configfile);
    socket._response.respLength = socket._response.response_string.size();

    // std::cout << "target : "<< socket._request.request_target << std::endl;
    // std::cout << "response is : \n+++++++++++++\n" << socket._response.response_string << "\n+++++++++\n" << std::endl;
    return 1;
}
