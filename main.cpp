/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:35:36 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/15 17:58:08 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include "./configfile/configfile.hpp"
#include "./server/socket.hpp"
#include "./request/INCLUDES/request.hpp"

void sigpipe_handler(int signum)
{
}

void	server_loop(std::vector<Socket> & sockets, std::vector<pfd> & pfds, ConfigFile & configFile, std::map<int, ConnectSocket> Connections)
{
	signal(SIGPIPE, sigpipe_handler);
	while (1)
	{
		poll(&pfds[0], pfds.size(), 0);
		for (size_t i = 0; i < pfds.size(); i++)
		{
			if (i >= sockets.size() && i < pfds.size() && getTimeOfNow() - Connections[pfds[i].fd].timeOut > 5)
			{
				checkTimeOut(pfds, Connections, configFile, i);
				i--;
			}
			if (pfds[i].revents & POLLIN)
			{
				if (i < sockets.size() && pfds[i].fd == sockets[i].getSocketId())
					pollin(pfds, sockets, Connections, i);
				else
				{
					Connections[pfds[i].fd].timeOut = getTimeOfNow();
					Connections[pfds[i].fd].readRequest(configFile);
					if (Connections[pfds[i].fd].closed)
					{
						if (Connections[pfds[i].fd]._response.response_string.size())
							sendError(pfds[i].fd, Connections[pfds[i].fd]._response.response_string);
						closeConnection(pfds, Connections, i);
						i--;
					}
				}
			}
			if (pfds[i].revents & POLLOUT)
			{
				pollout(pfds, Connections, i);
				if (Connections[pfds[i].fd].closed)
				{
					if (Connections[pfds[i].fd]._response.response_string.size())
						sendError(pfds[i].fd, Connections[pfds[i].fd]._response.response_string);
            		closeConnection(pfds, Connections, i);
					i--;
				}
				else if ((Connections[pfds[i].fd].conType && Connections[pfds[i].fd].ReadAvailble))
				{
            		closeConnection(pfds, Connections, i);
					i--;
				}
			}
			if (pfds[i].revents & (POLLERR | POLLHUP))
			{
				pollErrHup(pfds, Connections, i);
				i--;
			}
		}
	}	
}

void	start_server(std::string & _config)
{
	ConfigFile						configFile;
	std::vector<Socket>				sockets;
	std::vector<pfd>				pfds;
	std::map<int, ConnectSocket>	Connections;

	try
	{
		_config = read_file(_config);
		configFile = start_parse_config_file(_config);
		sockets = create_sockets(configFile);
		listenSocket(sockets);
		pfds = create_pfd(sockets);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << '\n';
		exit(EXIT_FAILURE);
	}
	server_loop(sockets, pfds, configFile, Connections);
}

int main(int argc, char **argv)
{
	if (argc > 2)
		return (std::cout << "INVALID ARGUMENTS" << std::endl, 1);
	std::string config_file = (argc == 2 ? argv[1] : "./tests/test.conf");
	start_server(config_file);	
	return (0);
}
