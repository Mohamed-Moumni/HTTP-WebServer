/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:53:14 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/28 08:53:30 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

void	print_listen(Server& vec)
{
	std::map<std::string, std::vector<std::string> >::iterator it = vec._listen.begin();
	std::cout << "####### HOSTS #######" << std::endl;
	std::cout << "Number of hosts : " << vec._listen.size() << std::endl;
	for (; it != vec._listen.end(); it++)
	{
		std::cout << "Host : " << it->first << "   Ports : ";
		std::vector<std::string> v = it->second;
		for (size_t i = 0; i < v.size(); i++)
		{
			std::cout << v[i] << " ";
		}
		std::cout << std::endl;
	}
}

void	print_server_names(Server& vec)
{
	std::cout << "####### SERVER NAMES #######" << std::endl;
	for (size_t i = 0; i < vec._server_names.size(); i++)
	{
		std::cout << vec._server_names[i] << " ";
	}
	std::cout << std::endl;
}

void	print_index(Server& vec)
{
	std::cout << "####### INDEX #######" << std::endl;
	for (size_t i = 0; i < vec._index.size(); i++)
	{
		std::cout << vec._index[i] << " ";
	}
	std::cout << std::endl;
}

void	print_error_pages(Server& vec)
{
	std::cout << "####### ERROR PAGES #######" << std::endl;
	std::map<std::string, std::string>::iterator it = vec._error_pages.begin();

	for (; it != vec._error_pages.end(); it++)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
}

void	print_locations(Server& serv)
{
	std::vector<location> loc = serv._locations;
	
	std::cout << "####### LOCATIONS #######" << std::endl;
	std::cout << "THIS SERVER HAS " << loc.size() << (loc.size() > 1 ? " LOCATIONS" : " LOCATION") << std::endl;
	for (size_t i = 0; i < loc.size(); i++)
	{
		location tmp = loc[i];
		std::cout << " -------------------------- " << std::endl;
		std::cout << "location number " << i+1 << std::endl;
		std::cout << "PATH " << tmp.path << std::endl;
		std::map<std::string, std::vector<std::string> >::iterator it = tmp._location_attr.begin();
		for (; it != tmp._location_attr.end(); it++)
		{
			std::cout << it->first << " ";
			print_vector(it->second);
		}
		std::cout << " -------------------------- " << std::endl;
	}
}

void	print_servers(std::vector<Server>& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (i) std::cout << std::endl;
		std::cout << "######## ####### ####### ######## " << std::endl;
		std::cout << "######## SERVER NUMBER " << i + 1 << " ########" << std::endl;
		std::cout << "######## ####### ####### ######## " << std::endl << std::endl;
		print_listen(vec[i]);
		print_server_names(vec[i]);
		print_index(vec[i]);
		print_error_pages(vec[i]);
		print_locations(vec[i]);
	}
}