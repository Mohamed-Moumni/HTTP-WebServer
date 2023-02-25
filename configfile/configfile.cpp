/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:54:45 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/25 10:22:24 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"

// void	check_scope()


void    start_parse_config_file(std::string config_file)
{
	check_syntax(config_file);
	start_parse(config_file);
}
