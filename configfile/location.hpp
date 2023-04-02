/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:51:55 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/02 13:54:43 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

class location
{
	public:
		std::string											path;
		std::string											_root;
		std::string											_autoindex;
		std::vector<std::string>							_index;
		std::vector<std::string>							_allowed_methods;
		std::map<std::string, std::string>					_error_pages;
		std::map<std::string, std::string>					_return;
};