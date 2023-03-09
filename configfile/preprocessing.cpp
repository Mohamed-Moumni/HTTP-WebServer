/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 11:02:35 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/28 12:12:25 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

bool	is_comment(std::string line)
{
	size_t i;

	for (i = 0; i < line.length(); i++)
		if (!isspace(line[i]))
			break;
	return (line[i] == '#' ? true : false);
}

std::string	preprocessing(std::string str)
{
	std::string	s = "";
	std::vector<std::string> v = str_split(str, '\n');

	for (size_t i = 0; i < v.size(); i++)
	{
		if (!is_comment(v[i]))
		{
			s += v[i];
			s += '\n';
		}
	}
	return s;
}