/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:35:36 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/27 11:05:28 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "./configfile/configfile.hpp"
#include "./server/socket.hpp"

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

void	start_server(std::string & _config)
{
	ConfigFile						configFile;
	std::vector<Socket>				sockets;
	std::vector<pfd>				pfds;
	std::map<int, ConnectSocket>	Connections;

	_config = read_file(_config);
	configFile = start_parse_config_file(_config);
	sockets = create_sockets(configFile);
}

int main(int argc, char **argv)
{
	if (argc > 2)
		return (std::cout << "INVALID ARGUMENTS" << std::endl, 1);
	std::string config_file = (argc == 2 ? argv[1] : "./tests/def.conf");
	start_server(config_file);	
	return (0);
}
