/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:53:34 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/25 10:19:34 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> str_split_spaces(std::string s, char c)
{
	std::vector<std::string>		v;
	std::string						str = "";

	for (int i = 0; i < s.length(); i++)
	{
		if (isspace(s[i]) || (i + 1 == s.length()))
		{
			if (isalnum(s[i]) && (i+1 == s.length()))
				str += s[i];
			std::string newstr = str_trim(str);
			if (newstr.length())
				v.push_back(newstr);
			str = "";
		}
		else
			str += s[i];
	}
	return v;
}

std::vector<std::string> str_split(std::string s, char c)
{
	std::vector<std::string>		v;
	std::string						str = "";

	if (c == ' ')
		return str_split_spaces(s, c);
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == c || (i + 1 == s.length()))
		{
			if (isalnum(s[i]) && (i+1 == s.length()))
				str += s[i];
			std::string newstr = str_trim(str);
			if (newstr.length())
				v.push_back(newstr);
			str = "";
		}
		else
			str += s[i];
	}
	return v;
}