/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf_file.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:49:02 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 10:19:24 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"
#include <stack>

void	exit_mode(std::string str)
{
	std::cout << "\033[31m" << "ERROR : " << str << std::endl;
	exit(1);
}

std::string		abstract_brackets(std::string str)
{
	size_t				i;
	std::string			brackets;

	i = 0;
	brackets = "";
	while (str[i])
	{
		if (str[i] == '}' || str[i] == '{')
			brackets += str[i];
		i++;
	}
	return brackets;
}

void	valid_brackets(std::string str)
{
	std::stack<char>	st;
	size_t				i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '{')
			st.push(str[i]);
		else if (str[i] == '}')
		{
			if (st.empty())
				exit_mode("INVALID BRACKETS");
			st.pop();
		}
		i++;
	}
	if (!st.empty())
		exit_mode("INVALID BRACKETS");
}

void	check_brackets(std::string str)
{
	std::string brackets = abstract_brackets(str);
	valid_brackets(brackets);
	std::cout << "Brackets valid" << std::endl;
}
