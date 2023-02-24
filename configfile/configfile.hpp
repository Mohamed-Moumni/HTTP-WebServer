/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:09:49 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/24 09:09:07 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

# include <vector>
# include <map>
# include "server.hpp"
class ConfigFile
{
	private:
		std::map<std::string, std::vector<std::string> >		_values;
		std::vector<Server>										_servers;
};

void    start_parse_config_file(std::string config_file);
void	check_brackets(std::string std);

#endif
