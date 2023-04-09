/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configfile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 21:09:49 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/09 12:26:34 by mkarim           ###   ########.fr       */
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
		std::map<std::string, std::string>						_mime_types;
		std::map<std::string, std::string>						_code_status;
		std::set<std::string>									_content_types;
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

template <typename T>
void	fill_allowed_methods(T& fill, std::vector<std::string>& vec);

template <typename T>
void	fill_return(T& fill, std::vector<std::string>& vec);

template <typename T>
void	fill_error_pages(T& fill, std::vector<std::string>& vec);

template <typename T>
void	fill_index(T& fill, std::vector<std::string>& vec);

template <typename T>
void	fill_server_name(T& fill, std::vector<std::string>& vec);

void	fill_server_attr(Server& serv, std::vector<std::string>& vec);


#endif
