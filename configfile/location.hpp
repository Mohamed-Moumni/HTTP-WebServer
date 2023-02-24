/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:51:55 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 08:55:09 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

class location
{
	private:
		std::map<std::string, std::string>		_location_attr;
};