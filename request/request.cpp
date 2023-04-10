#include "INCLUDES/request.hpp"
#include <fstream> //for ifstream

int request_handler(ConnectSocket & socket, ConfigFile configfile)
{
    if(!pars_request(socket._request) || !possible_error(socket, configfile))
    {
        socket._response.response_string = respond_error("400", configfile);
		socket._response.respLength = socket._response.response_string.size();
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