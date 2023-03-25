/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_errors.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:49:02 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/25 14:22:55 by mkarim           ###   ########.fr       */
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
}

// void	check_scope(std::string str)
// {
// 	std::string			buff;
// 	size_t				i;

// 	buff = "";
// 	i = 0;
// 	while (str[i])
// 	{
		
// 	}
// }

bool	is_has_bracket(std::string s)
{
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == '}' || s[i] == '{')
			return true;
	}
	return false;
}

size_t	num_of_semicolon(std::string s)
{
	size_t	count;

	count = 0;
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == ';')
			count++;
	}
	return count;
}

void	check_semicolon(std::string config_file)
{
	std::vector<std::string> v = str_split(config_file, '\n');
	for (size_t i = 0; i < v.size(); i++)
	{
		if (is_has_bracket(v[i]) && num_of_semicolon(v[i]))
			exit_mode("SEMICOLON ISSUE, EXTRA SEMICOLON");
		else if (!is_has_bracket(v[i]) && v[i][v[i].size() - 1] != ';')
			exit_mode("SEMICOLON ISSUE, SHOULD BE THE LAST CHARACTER");
		else if (!is_has_bracket(v[i]) && num_of_semicolon(v[i]) != 1)
			exit_mode("SEMICOLON ISSUE, EXTRA SEMICOLON, SHOULD BE ONE PER LINE");
	}
}

void	check_syntax(std::string config_file)
{
    check_brackets(config_file);
	check_semicolon(config_file);
	// check_scope(config_file);
}
