/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:53:34 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 17:13:15 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> str_split(std::string s, char c)
{
    std::vector<std::string> v;
    std::string str = "";

    for (int i = 0; i < s.length(); i++)
    {
        str += s[i];
        if (s[i] == c || (i + 1 == s.length()))
        {
            std::string newstr = str_trim(str);
            if (newstr.length())
                v.push_back(newstr);
            str = "";
        }
    }
    return v;
}