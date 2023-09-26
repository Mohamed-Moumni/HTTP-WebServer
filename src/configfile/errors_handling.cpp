/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 14:41:48 by mkarim            #+#    #+#             */
/*   Updated: 2023/09/26 09:08:54 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/configfile.hpp"

void	listen_errors(Server& serv)
{
	std::map<std::string, std::set<std::string> >::iterator it;

	if (!serv._listen.size())
		exit_mode("ADD HOST:PORT");
	for (it = serv._listen.begin(); it != serv._listen.end(); it++)
	{
		std::vector<std::string> host = str_split(it->first, '.');
		if (host.size() != 4)
			exit_mode("INVALID HOST!");
		std::set<std::string> ports = it->second;
		for (std::set<std::string>::iterator itr = ports.begin(); itr != ports.end(); itr++)
		{
			if ((*itr).length() != 4)
				exit_mode("INVALID PORT!");
		}
	}
}

void	errors_handling(std::vector<Server>& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		listen_errors(servers[i]);
	}
}