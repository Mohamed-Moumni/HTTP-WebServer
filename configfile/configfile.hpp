/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:09:49 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/25 10:22:48 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_FILE_HPP
# define CONFIG_FILE_HPP

# include <vector>
# include <map>
# include <stack>
# include "server.hpp"
# include "../utils/utils.hpp"
# include <utility>
class ConfigFile
{
	public:
		std::map<std::string, std::vector<std::string> >		_values;
		std::vector<Server>										_servers;
};

void    start_parse_config_file(std::string config_file);
void	check_syntax(std::string config_file);
void	check_brackets(std::string std);
void	start_parse(std::string config_file);
void	exit_mode(std::string str);
void	print_servers(std::vector<Server>& vec);

#endif
