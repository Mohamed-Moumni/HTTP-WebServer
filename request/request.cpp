#include "INCLUDES/request.hpp"
#include <fstream> //for ifstream

int possible_error(ConnectSocket &socket)
{
    //todo
    return 1;
}

int request_handler(ConnectSocket & socket, ConfigFile configfile)
{

    if(!pars_request(socket._request))
        return 0;
    if(!possible_error(socket))
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

    socket._request.request_string  = "GET /test/index.html HTTP/1.1\r\nHost: unknownserver\r\nConnection: close\r\n\r\nhello everybody here is the body";
    std::fstream out_file;
    out_file.open("out.html");

    if(!request_handler(socket, configfile))
    {
        std::cout << "request error" << std::endl;
        return 0;
    }

    respond(socket, configfile);

    std::cout << "the response is: "<< socket._response.response_string << std::endl;

    for(int i = 0; i < configfile._servers.size(); i++)
        std::cout << configfile._servers[i]._server_names[0] << std::endl;
}