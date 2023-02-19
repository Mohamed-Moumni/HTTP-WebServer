/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_trim.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 16:56:44 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 17:05:53 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int		get_end_index(std::string s)
{
	int i = s.length() - 1;
	while (i >= 0 && isspace(s[i]))
		i--;
	return i+1;
}

std::string str_trim(std::string s)
{
	std::string str = "";
	int			i;
	int			end_ind;

	i = 0;
	end_ind = get_end_index(s);
	while (isspace(s[i]))
		i++;
	for (; i < end_ind; i++)
		str += s[i];
	return str;
}