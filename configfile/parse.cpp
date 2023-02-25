/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:51:19 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/25 16:21:41 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"
#include <string>

size_t		exclude_bracket(std::string str, size_t pos)
{
	std::stack<char> st;

	st.push(str[pos]);
	while (str[pos])
	{
		if (st.empty())
			break;
		if (str[pos] == '{')
			st.push(str[pos]);
		else if (str[pos] == '}')
			st.pop();
		pos++;
	}
	return pos;
}

bool	line_has_brack(std::string str, size_t& pos)
{
	int i = pos;
	while (str[i] && str[i] != '\n')
	{
		if (str[i] == '{')
		{
			pos = i+1;
			return true;
		}
		i++;
	}
	return false;
}

std::string		data_of_server(std::string str, size_t pos)
{
	std::string 		buff;

	buff = "";
	while (str[pos] && str[pos] != '{')
		pos++;
	pos++;
	while (str[pos] != '}')
	{
		if (line_has_brack(str, pos))
			pos = exclude_bracket(str, pos);
		else
			buff += str[pos];
		pos++;
	}
	return str_trim(buff);
}

void	fill_server_name(Server& serv, std::vector<std::string>& vec)
{
	for (size_t i = 1; i < vec.size(); i++)
		serv._server_names.push_back(vec[i]);
}

void	fill_listen(Server& serv, std::vector<std::string>& vec)
{
	for (size_t i = 1; i < vec.size(); i++)
	{
		std::vector<std::string> host_port = str_split(vec[i], ':');
		if (host_port.size() != 2)
			exit_mode("INVALID LISTEN ARGS");
		if (serv._listen.count(host_port[0]))
			serv._listen[host_port[0]].push_back(host_port[1]);
	
		else
		{
			std::vector<std::string> v;
			v.push_back(host_port[1]);
			serv._listen.insert(std::make_pair(host_port[0], v));
		}
	}
}

void	fill_index(Server& serv, std::vector<std::string>& vec)
{
	for (size_t i = 1; i < vec.size(); i++)
		serv._index.push_back(vec[i]);
}

void	fill_error_pages(Server& serv, std::vector<std::string>& vec)
{
	if (vec.size() < 3)
		exit_mode("LESS ERROR PAGES ARGS");
	for (size_t i = 1; i < vec.size() - 1; i++)
	{
		serv._error_pages.insert(std::make_pair(vec[i], vec[vec.size() - 1]));
	}
}

void	fill_server_attr(Server& serv, std::vector<std::string>& vec)
{
	std::string		attr = vec[0];
	if (attr == "server_name")
		fill_server_name(serv, vec);
	else if (attr == "listen")
		fill_listen(serv, vec);
	else if (attr == "index")
		fill_index(serv, vec);
	else if (attr == "error_page")
		fill_error_pages(serv, vec);
	else if (attr == "client_max_body_size")
		serv._client_max_body_size = vec[1];
	else if (attr == "root")
		serv._root = vec[1];
	else
	{
		exit_mode("SOMETHING WRONG");
	}
}

void	fill_server(Server& serv, std::string str)
{
	std::vector<std::string> vec = str_split(str, '\n');
	for (size_t i = 0; i < vec.size(); i++)
	{
		std::vector<std::string> test = str_split(vec[i], ' ');
		fill_server_attr(serv, test);
	}
}

void	edit_check_var(std::string s, int& check)
{
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == '{' || s[i] == '}')
		{
			(s[i] == '{' ? ++check : --check);
			return ;
		}
	}
}

std::string		data_from_pos(std::string s, size_t pos)
{
	std::string res = "";

	for (size_t i = pos; i < s.length(); i++)
	{
		res += s[i];
	}
	return res;
}

// std::string		get_data_of_scope(std::string str, size_t pos)
// {
// 	std::vector<std::string>	vec = str_split(data_from_pos(str, pos), '\n');
// 	std::string		res;
// 	int				check;
// 	int				prev;
// }

std::vector<std::string>	data_of_locations(std::string str)
{
	std::vector<std::string>	vec = str_split(str, '\n');
	std::vector<std::string>	data;
	std::string					s;
	int							check;
	int							prev;

	check = 0;
	s = "";
	std::cout << "------------------" << std::endl;
	for (size_t i = 0; i < vec.size(); i++)
	{
		// std::cout << vec[i] << std::endl;
		prev = check;
		if (is_has_bracket(vec[i]))
			edit_check_var(vec[i], check);
		std::cout << i << " " << check << std::endl;
		if (prev == check && check == 2)
		{
			s += vec[i];
			s += '\n';
		}  
		if (check == 0)
		{
			// std::cout << "I WILL PUSH" << std::endl;
			// std::cout << s << std::endl;
			data.push_back(s);
			s = "";
		}
	}
	std::cout << "------------------" << std::endl;
	return data;
}

Server		parse_one_server(std::string str, size_t pos)
{
	Server			serv;
	std::string		serv_data;
	location		loc;
	std::vector<std::string>		loc_data;
	
	serv_data = data_of_server(str, pos);
	fill_server(serv, serv_data);
	loc_data = data_of_locations(data_from_pos(str, pos));
	for (size_t i = 0; i < loc_data.size(); i++)
	{
		// std::cout << loc_data[i] << std::endl;
	}
	// serv._locations = fill_location(loc_data);
	return serv;
}

std::vector<Server>	parse_servers(std::string str)
{
	std::vector<Server>		_vec_serv;
	std::string				line;
	size_t					offset;

	offset = str.find("server{");
	while (offset != std::string::npos)
	{
		_vec_serv.push_back(parse_one_server(str, offset));
		offset = str.find("server{", offset + 1);
	}
	// print_servers(_vec_serv);
	return _vec_serv;
}

void	start_parse(std::string config_file)
{
	ConfigFile		conf;
	std::string		serv;

	conf._servers = parse_servers(config_file);
}