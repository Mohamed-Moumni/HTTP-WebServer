/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:51:19 by mkarim            #+#    #+#             */
/*   Updated: 2023/03/31 14:05:30 by mkarim           ###   ########.fr       */
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
		{
			serv._listen[host_port[0]].insert(host_port[1]);
		}
		else
		{
			std::set<std::string> v;
			v.insert(host_port[1]);
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

void	fill_allowed_methods(Server& serv, std::vector<std::string>& vec)
{
	std::vector<std::string>	methods = str_split(vec[1], ',');
	for (size_t i = 0; i < methods.size(); i++)
	{
		serv._allowed_methods.push_back(methods[i]);
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
	else if (attr == "autoindex")
		serv._autoindex = vec[1];
	else if (attr == "upload")
		serv._upload = vec[1];
	else if (attr == "allowed_methods")
		fill_allowed_methods(serv, vec);
	else
	{
		std::cout << attr << std::endl;
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

/* ########  EDIT ON CHECK VARIABLE IN "DATA OF LOCATIONS" FUNCTION TO DETERMINE THE LOCATION SCOPE ######## */
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

/* ########  GET THE DATA FROM POS UNTIL END OF STR ######## */
std::string		data_from_pos(std::string s, size_t pos)
{
	std::string res = "";

	for (size_t i = pos; i < s.length(); i++)
	{
		res += s[i];
	}
	return res;
}

std::string		net_data_of_scope(std::string str)
{
	std::string		res = "server{";
	size_t			i = 0;
	size_t			check = 1;
	while (str[i] != '{')
		i++;
	while (str[++i])
	{
		if (str[i] == '{')
			check++;
		else if (str[i] == '}')
			check--;
		res += str[i];
		if (!check)
			break;
	}
	return res;
}

bool	has_open_brack(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '{')
			return true;
	}
	return false;
}

bool	has_closed_brack(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '}')
			return true;
	}
	return false;
}

std::vector<std::string>	locations_data(std::string str)
{
	std::vector<std::string> res;
	std::vector<std::string> vec = str_split(str, '\n');
	std::string loc = "";
	size_t		add = 0;
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (has_open_brack(vec[i]))
			add++;
		if (add == 2)
		{
			loc += vec[i];
			loc += '\n';
		}
		if (has_closed_brack(vec[i]))
		{
			add--;
			res.push_back(loc);
			loc = "";
		}
	}
	return res;
}

std::vector<std::string>	get_data_of_scope(std::string str)
{
	std::vector<std::string> loc;
	
	str = net_data_of_scope(str);
	loc = locations_data(str);
	return loc;
}

std::string		abstract_path(std::string data)
{
	std::string str;
	std::string res = " ";;

	str = data;
	for (size_t i = str.length() - 2; i >= 0; i--)
	{
		res += str[i];
		if (isspace(str[i]) && str_trim(res).length())
			break;
	}
	reverse(res.begin(), res.end());
	return str_trim(res);
}

void	print_vector(std::vector<std::string>& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

/* ######## PARSE ONE LOCATION ######## */
location	parse_one_location(std::string data)
{
	location	loc;
	std::vector<std::string> vec = str_split(data, '\n');
	std::vector<std::string> v;
	
	loc.path = abstract_path(vec[0]);
	for (size_t i = 1; i < vec.size(); i++)
	{
		std::vector<std::string> line = str_split(vec[i], ' ');
		for (size_t j = 1; j < line.size(); j++)
		{
			v.push_back(line[j]);
		}
		loc._location_attr.insert(std::make_pair(line[0], v));
		v.clear();
	}
	return loc;
}

/* ######## FILL_LOCATIONS OF SERVER ######## */
std::vector<location>	fill_location(std::vector<std::string>& data)
{
	std::vector<location>	locations;
	for (size_t i = 0; i < data.size(); i++)
	{
		locations.push_back((parse_one_location(str_trim(data[i]))));
	}
	return locations;
}

/* ########  GET THE DATA OF ALL LOCATIONS INSIDE THE STR ######## */
std::vector<std::string>	data_of_locations(std::string str)
{
	std::vector<std::string>	vec = str_split(str, '\n');
	std::vector<std::string>	data;
	std::string					s;
	int							check;
	int							prev;

	check = 0;
	s = "";
	for (size_t i = 0; i < vec.size(); i++)
	{
		prev = check;
		if (is_has_bracket(vec[i]))
			edit_check_var(vec[i], check);
		if (prev == check && check == 2)
		{
			s += vec[i];
			s += '\n';
		}  
		if (check == 0)
		{
			data.push_back(s);
			s = "";
		}
	}
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
	// std::cout << "----- data -----" << std::endl;
	// std::cout << serv_data << std::endl;
	loc_data = get_data_of_scope(data_from_pos(str, pos));
	serv._locations = fill_location(loc_data);
	return serv;
}

bool	is_server_block(std::string str, size_t pos)
{
	str = data_from_pos(str, pos + 6);
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isspace(str[i]))
			return str[i] == '{';
	}
	return false;
}

void	move_offset_to_next_server_block(std::string str, size_t &offset)
{
	size_t i = -1;
	size_t bracket = 0;
	while (str[++i])
	{
		if (bracket != 0)
			break;
		if (str[i] == '{')
			bracket++;
		else if (str[i] == '}')
			bracket--;
	}
	if (bracket < 0)
		exit_mode("FOUND CLOSE BRACKET");
	while (str[++i])
	{
		if (bracket == 0)
			break;
		if (str[i] == '{')
			bracket++;
		else if (str[i] == '}')
			bracket--;
	}
	offset += i;
}

std::vector<Server>	parse_servers(std::string str)
{
	std::vector<Server>		_vec_serv;
	std::string				line;
	size_t					offset;

	offset = str.find("server");
	if (offset)
		exit_mode("DAKHAL SERVER M9AD MN ZAMLA DYAL WALOU");
	while (offset != std::string::npos)
	{
		if (is_server_block(str, offset))
		{
			_vec_serv.push_back(parse_one_server(str, offset));
			move_offset_to_next_server_block(str, offset);
		}
		offset = str.find("server", offset + 1);
	}
	return _vec_serv;
}

ConfigFile	start_parse(std::string config_file)
{
	ConfigFile		conf;
	std::string		serv;

	conf._servers = parse_servers(config_file);
	errors_handling(conf._servers);
	print_servers(conf._servers);
	exit(0);
	return (conf);
}