#include "INCLUDES/request.hpp"
#include <fstream>

int request_handler(ConnectSocket & socket, ConfigFile configfile)
{
	int ret;
	// std::cout << socket._request.request_string << std::endl;

    if(!pars_request(socket._request) || !(ret = possible_error(socket, configfile)))
    {
        socket._response.response_string = respond_error("400", configfile);
		socket._response.respLength = socket._response.response_string.size();
		return 0;
    }
	else if(ret == 2)//this means that there is an error but it's not 400. so it's handlend inside possible error function  
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