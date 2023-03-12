/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:53:34 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/25 16:24:13 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> str_split_spaces(std::string s)
{
	std::vector<std::string>		v;
	std::string						str = "";

	for (size_t i = 0; i < s.length(); i++)
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
		return str_split_spaces(s);
	for (size_t i = 0; i < s.length(); i++)
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

std::vector<std::string> str_split(std::string s, std::string c)
{
	std::vector<std::string> ret_vec;
	int start = 0;
	if(!s.size())
		return ret_vec;
	if(c.size() == 1)
		return str_split(s, (char)c[0]);
	for(int i = 0; i < (s.size() - c.size() + 1) ; i++)
	{
		if(s.substr(i, c.size()) == c)
		{
			ret_vec.push_back(s.substr(start, i - start));
			start = i + c.size();
		}
	}
	if(start <= s.size()-1)
		ret_vec.push_back(s.substr(start, s.size() - start));
	
	return ret_vec;
}
// int main()
// {
// 	std::string str = "aaa;;bbb;;ccc;;ddd";
// 	std::string split = ";";
// 	std::vector<std::string> ret = str_split(str, ';');
// 	for(int i = 0; i < ret.size(); i++)
// 		std::cout << ret[i] << std::endl;
// }