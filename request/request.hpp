/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 10:39:03 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 17:15:30 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "../utils/utils.hpp"

class Request {
	public:
		std::string												_request_line;
		std::string												_request_method;
		std::string												_path_component;
		std::string												_http_version;
		
		std::map<std::string, std::vector<std::string> >		_request_headers;
};

void	start_request(std::string request);

#endif 
