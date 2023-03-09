/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_handling.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 14:41:48 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/09 15:15:38 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

void	listen_errors(Server& serv)
{
	std::map<std::string, std::vector<std::string> >::iterator it;

	if (!serv._listen.size())
		exit_mode("ADD HOST:PORT");
	for (it = serv._listen.begin(); it != serv._listen.end(); it++)
	{
		std::vector<std::string> host = str_split(it->first, '.');
		if (host.size() != 4)
			exit_mode("INVALID HOST!");
		std::vector<std::string> v = it->second;
		for (size_t i = 0; i < v.size(); i++)
		{
			if (v[i].length() != 4)
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