#include <iostream>
#include <fstream>
#include <deque>
#include "../utils/utils.hpp"
#include "../configfile/configfile.hpp"
#include "../server/ConnectSocket.hpp"

int prefix_match(std::string s1, std::string s2)
{
    int minm = std::min(s1.size(), s2.size());
    int i = 0;

    while(i < minm && s1[i] == s2[i])
        i++;
    return i;
}

int get_request_line(request &request)
{
    int i = 0;
    std::vector<std::string> start_line;

    while(request.request_string[i] && request.request_string.substr(i, 2) != "\r\n")
        i++;
    start_line = str_split_spaces(request.request_string.substr(0, i));
    if(start_line.size() != 3)
        return 0;
    if(request.request_string[i+1])
        request.request_string = request.request_string.substr(i + 2, request.request_string.size() - 1);
    request.method = start_line[0];
    request.request_target = start_line[1];
    request.http_version = start_line[2];
    return 1;
}

int get_request_headers(request &request)
{
    std::vector<std::string> header_lines;
    std::vector<std::string> key_value;
    std::string header_string;

    if(request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        header_string = request.request_string.substr(0, request.request_string.find("\r\n\r\n"));
        request.request_string = request.request_string.substr(request.request_string.find("\r\n\r\n") + 4);
    }
    else
        return 0;
    header_lines = str_split(header_string, "\r\n");
    for(size_t i = 0; i < header_lines.size(); i++)
    {
        key_value.clear();
        key_value = header_spliter(header_lines[i]);
        std::cout << key_value.size() << std::endl;
        if(key_value.size() != 2)
            return 0;
        request.headers_map[key_value[0]] = key_value[1];    
    } 
    return 1;
}

int get_request_body(request &request)
{
    request.request_body = request.request_string;
    return 1;
}

int pars_request(request &request)
{
    if(!get_request_line(request) && !get_request_headers(request) && !get_request_body(request))
        return 0;
    if(!get_request_headers(request))
        return 0;
    if(!get_request_body(request))
        return 0;
    return 1;
}

int possible_error(ConnectSocket &socket)
{
    //todo
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

    find_server(socket, configfile, server);
    find_location(socket, server, location);

    std::cout << "the chosen server is : " << server._server_names[0] << std::endl;
    std::cout << "the chosen location is : " << location.path << std::endl;
    return 1;
}

int request_handler(ConnectSocket & socket, ConfigFile configfile)
{
    (void)(configfile);
    if(!pars_request(socket._request))
        return 0;
    if(!possible_error(socket))
        return 0;
    if(!respond(socket, configfile))
        return 0;

    return 1;
}

std::string		read_file(std::string file_name)
{
	std::string	data;
	std::string	tmp;

	std::ifstream	file(file_name);
	while (getline(file, tmp))
	{
		data += tmp;
		data += "\n";
	}
	return data;
}

int main()
{
    ConnectSocket socket;
    socket.IpAdress = "127.0.0.1";
    socket.Port = "8080";
    ConfigFile configfile = start_parse_config_file(read_file("../tests/def.conf"));
    // std::cout << configfile._servers[0]._index[0] << std::endl;

    socket._request.request_string  = "GET /ph HTTP/1.1\r\nHost: unknownserver\r\nConnection: close\r\n\r\nhello everybody here is the body";
    std::fstream out_file;
    out_file.open("out.html");

    // if(!request_handler(socket, configfile))
    // {
    //     std::cout << "request error" << std::endl;
    //     return 0;
    // }

    for(int i = 0; i < configfile._servers.size(); i++)
        std::cout << configfile._servers[i]._server_names[0] << std::endl;

    // out_file << response << std::endl;
    // std::cout << "HOST:" << socket._request.headers_map["Host"] << std::endl;
    // std::cout << "User-Agent:" << socket._request.headers_map["User-Agent"] << std::endl;
    // std::cout << "Accept:" << socket._request.headers_map["Accept"] << std::endl;
    // std::cout << "Connection:" << socket._request.headers_map["Connection"] << std::endl;
    // std::cout << "+++" << socket._request.request_body << "+++" << std::endl;
}