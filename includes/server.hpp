/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:53:21 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/16 13:39:10 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "location.hpp"

class Server
{
	public:
		std::map<std::string, std::set<std::string> > 		_listen;
		std::string											_root;
		std::string											_autoindex;
		size_t												_client_max_body_size;
		std::vector<std::string>							_server_names;
		std::vector<std::string>							_index;
		std::vector<std::string>							_allowed_methods;
		std::vector<location>								_locations;
		std::map<std::string, std::string>					_error_pages;
		std::map<std::string, std::string>					_custom_error_pages;

	public:
		Server();
		~Server();
};
