/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 16:53:14 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/11 13:16:20 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

void	print_listen(Server& vec)
{
	std::map<std::string, std::set<std::string> >::iterator it = vec._listen.begin();
	std::cout << "####### HOSTS #######" << std::endl;
	std::cout << "Number of hosts : " << vec._listen.size() << std::endl;
	for (; it != vec._listen.end(); it++)
	{
		std::cout << "Host : " << it->first << "   Ports : ";
		std::set<std::string> ports = it->second;
		for (std::set<std::string>::iterator itr = ports.begin(); itr != ports.end(); itr++)
		{
			std::cout << *itr << " ";
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

template <typename T>
void	print_index(T& fill)
{
	if (!fill._index.size())
		return ;
	std::cout << "####### INDEX #######" << std::endl;
	for (size_t i = 0; i < fill._index.size(); i++)
	{
		std::cout << fill._index[i] << " ";
	}
	std::cout << std::endl;
}

template <typename T>
void	print_error_pages(T& fill)
{
	if (!fill._error_pages.size())
		return ;
	std::cout << "####### ERROR PAGES #######" << std::endl;
	std::map<std::string, std::string>::iterator it = fill._error_pages.begin();

	for (; it != fill._error_pages.end(); it++)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
}

template <typename T>
void	print_return(T& fill)
{
	if (!fill._return.size())
		return ;
	std::cout << "return : "  << fill._return << std::endl;
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
		std::cout << "UPLOAD " << tmp._upload << std::endl;
		std::cout << "Root is : " << tmp._root << std::endl;
		std::cout << "AutoIndex is : " << tmp._autoindex << std::endl;
		print_index(tmp);
		// print_error_pages(tmp);
		print_return(tmp);
		std::map<std::string, std::vector<std::string> >::iterator it = tmp._others.begin();
		for (; it != tmp._others.end(); it++)
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
		std::cout << "_root is : " << vec[i]._root << std::endl;
		std::cout << "_autoindex is : " << vec[i]._autoindex << std::endl;
		std::cout << "_client_max_body_size is : " << vec[i]._client_max_body_size << std::endl;
		// std::cout << "_upload is : " << vec[i]._upload << std::endl;
		print_listen(vec[i]);
		print_server_names(vec[i]);
		print_index(vec[i]);
		print_error_pages(vec[i]);
		print_locations(vec[i]);
	}
}