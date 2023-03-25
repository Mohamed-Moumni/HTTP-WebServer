/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:54:45 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/25 06:11:39 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

ConfigFile	start_parse_config_file(std::string config_file)
{
	config_file = preprocessing(config_file);
	check_syntax(config_file);
	return (start_parse(config_file));
}
