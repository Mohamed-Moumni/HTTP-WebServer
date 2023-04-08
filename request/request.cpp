#include "INCLUDES/request.hpp"
#include <fstream> //for ifstream

int request_handler(ConnectSocket & socket, ConfigFile configfile)
{
    if(!pars_request(socket._request) || !possible_error(socket, configfile))
    {
        socket._response.response_string = respond_error("400");
		return 0;
    }
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

// std::string		read_request(std::string file_name)
// {
// 	std::string	data;
// 	std::string	tmp;

// 	std::ifstream	file(file_name);
// 	while (getline(file, tmp))
// 	{
// 		data += tmp;
// 		data += "\r\n";
// 	}
// 	return data;
// }

// int main()
// {
//     ConnectSocket socket;
//     socket.IpAdress = "127.0.0.1";
//     socket.Port = "8080";
//     ConfigFile configfile = start_parse_config_file(read_file("../tests/def.conf"));
//     socket._request.request_string  = read_request("../tests/request.txt");

//     std::fstream out_file;
//     out_file.open("out.html");

//     if(!request_handler(socket, configfile))
//     {
//         std::cout << "request error" << std::endl;
// 		std::cout << "the response is: "<< socket._response.response_string << std::endl;
//         return 0;
//     }
// 	std::cout << "method: "<< socket._request.method << std::endl;
//     respond(socket, configfile);

//     std::cout << "the response is: \n\n\n------------------------\n"<< socket._response.response_string <<"------------------------\n" << std::endl;
// }