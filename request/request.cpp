/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:54:14 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 18:44:18 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include <sstream>

void    fill_request_line(Request &req, std::string& data)
{
	req._request_line = data;
	std::vector<std::string> v = str_split(data, ' ');
	req._request_method = v[0];
	req._path_component = v[1];
	req._http_version   = v[2];
}

void	print(Request &req)
{
	std::cout << "request method " << req._request_method << std::endl;
	std::cout << "component      " << req._path_component << std::endl;
	std::cout << "http version   " << req._http_version << std::endl;
	
	std::map<std::string, std::vector<std::string> >::iterator it;

	it = req._request_headers.begin();
	for (; it != req._request_headers.end(); it++)
	{
		std::cout << it->first << " ";
		std::vector<std::string> v = it->second;
		for (int i = 0; i < v.size(); i++)
		{
			std::cout << "|" << v[i] << "|";
		}
		std::cout << std::endl;
	}
}

void    start_request(std::string request)
{
	Request			my_request;
	std::string		data;

	std::stringstream ss(request);

	int i = 0;
	getline(ss, data, '\n');
	fill_request_line(my_request, data);
	while (getline(ss, data, '\n'))
	{
		if (str_trim(data).length() > 0)
		{
			std::vector<std::string> line = str_split(data, ':');
			my_request._request_headers.insert(std::make_pair(line[0], str_split(line[1], ',')));
		}
	}
	print(my_request);
}