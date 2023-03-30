/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:09:49 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/29 17:14:42 by mmoumni          ###   ########.fr       */
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
		std::vector<Server>										_servers;
};

ConfigFile		start_parse_config_file(std::string config_file);
void			check_syntax(std::string config_file);
void			check_brackets(std::string std);
ConfigFile		start_parse(std::string config_file);
void			exit_mode(std::string str);
void			print_servers(std::vector<Server>& vec);
bool			is_has_bracket(std::string s);
void			print_vector(std::vector<std::string>& v);
std::string		preprocessing(std::string str);
void			errors_handling(std::vector<Server>& servers);

#endif
