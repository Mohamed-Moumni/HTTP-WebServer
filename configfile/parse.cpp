/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:51:19 by mkarim            #+#    #+#             */
/*   Updated: 2023/04/16 13:58:46 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configfile.hpp"
#include "../request/INCLUDES/request.hpp"
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

template <typename T>
void	fill_server_name(T& fill, std::vector<std::string>& vec)
{
	for (size_t i = 1; i < vec.size(); i++)
		fill._server_names.push_back(vec[i]);
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
			if (serv._listen[host_port[0]].count(host_port[1]))
				exit_mode("DON'T REPEAT THE SAME HOST:PORT");
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

template <typename T>
void	fill_index(T& fill, std::vector<std::string>& vec)
{
	if (vec.size() < 2)
		exit_mode("INVALID NUMBER OF ARGS OF INDEX");
	for (size_t i = 1; i < vec.size(); i++)
		fill._index.push_back(vec[i]);
}

std::string	read_ifstream(std::ifstream& file)
{
	std::string	data = "";
	std::string tmp = "";
	
	while (getline(file, tmp))
	{
		data += tmp;
		data += '\n';
	}
	return data;
}

template <typename T>
void	fill_error_pages(T& fill, std::vector<std::string>& vec)
{
	std::ifstream file;

	if (vec.size() < 3)
		exit_mode("LESS ERROR PAGES ARGS");
	for (size_t i = 1; i < vec.size() - 1; i++)
	{
		file.clear();
		file.open(vec[vec.size() - 1]);
		if (!file)
			exit_mode("FILE OF ERROR PAGE DOESN'T EXIST");
		fill._custom_error_pages.insert(std::make_pair(vec[i], read_ifstream(file)));
		fill._error_pages.insert(std::make_pair(vec[i], vec[vec.size() - 1]));
	}
}

template <typename T>
void	fill_return(T& fill, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		exit_mode("INVALID RETURN ARGS");
	fill._return = vec[1];
}

template <typename T>
void	fill_allowed_methods(T& fill, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		exit_mode("ALLOWED METHODS ERROR, MAEK SURE TO SEPARATE ALLOWED METHODS BY COMMA");
	std::vector<std::string>	methods = str_split(vec[1], ',');
	for (size_t i = 0; i < methods.size(); i++)
	{
		fill._allowed_methods.push_back(methods[i]);
	}
}

bool	is_body_size_not_valid(std::string s)
{
	// std::string max = "2000";

	if (s[s.length() - 1] != 'm')
		exit_mode("BODY SIZE SHOULD BE LIKE THIS '0000m'");
	if (s.length() > 5)
		return true;
	if (stoi(s) > 2000 || stoi(s) < 1)
		exit_mode("BODY_SIZE SHOULD BE BETWEEN 1-2000");
	return false;
}

void	fill_string_attr(Server& serv, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		exit_mode("NUM OF ARGS IS NOT VALID CHECK(ROOT/AUTOINDEX/CLIENT__SIZE)");
	if (vec[0] == "client_max_body_size")
	{
		if (vec[1][0] == '-')
			exit_mode("CLIENT MAX BODY SIZE SHOULD BE POSITIVE NUMBER");
		if (is_body_size_not_valid(vec[1]))
			exit_mode("CLIENT MAX BODY SIZE SHOULD BE LOWER THAN 2000");
		serv._client_max_body_size = stoi(vec[1]) * 1000000;
	}
	else if (vec[0] == "root")
		serv._root = vec[1];
	else if (vec[0] == "autoindex")
		serv._autoindex = vec[1];
	// else if (vec[0] == "upload")
	// 	serv._upload = vec[1];
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
	else if (attr == "client_max_body_size" || attr == "root" || attr == "autoindex" || attr == "upload")
		fill_string_attr(serv, vec);
	else if (attr == "allowed_methods")
		fill_allowed_methods(serv, vec);
	else
		exit_mode("INVALID ARG FOR SERVER BLOCK");
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

void	check_path_validity(std::string data, std::string path)
{
	std::string s = "";

	for (size_t i = 0; i < data.length(); i++)
	{
		if (data[i] == '{')
			break;
		s += data[i];
	}
	if (!str_split(str_trim(s), ' ').size())
		exit_mode("DIR PATH M9AD MN ZAMLA");
	if (path[0] != '/')
		exit_mode("THE PATH OF LOCATION SHOULD START WITH '/'");
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
	res = str_trim(res);
	check_path_validity(data, res);
	return res;
}

void	print_vector(std::vector<std::string>& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

void	fill_root_autoindex_upload(location& loc, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		exit_mode("INVALID NUMBER OF ARGS CHECK (ROOT/AUTOINDEX/UPLOAD)");
	if (vec[0] == "root")
		loc._root = vec[1];
	else if (vec[0] == "autoindex")
		loc._autoindex = vec[1];
	else if (vec[0] == "upload")
		loc._upload = vec[1];
}

void	fill_others(location& loc, std::vector<std::string>& vec)
{
	std::vector<std::string> v;
	for (size_t j = 1; j < vec.size(); j++)
	{
		v.push_back(vec[j]);
	}
	loc._others.insert(std::make_pair(vec[0], v));
}

void	check_and_set_cgi_path(location& loc, std::string s)
{
	std::ifstream file;

	file.open(s);
	if (!file)
		exit_mode("CGI PATH DOESN'T EXIST");
	loc._cgiPath = s;
}

void	fill_cgi_attr(location& loc, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		exit_mode("INVALID ARGUMENT OF CGI ATTRIBUTES");
	if (vec[0] == "cgiPath")
		check_and_set_cgi_path(loc, vec[1]);
	else if (vec[0] == "cgiExt")
	{
		if (vec[1] != ".php" && vec[1] != ".py")
			exit_mode("INVALID EXTENSION");
		loc._cgiExt = vec[1];
	}
}

void	fill_location_attr(location& loc, std::string& s)
{
	std::vector<std::string> vec = str_split(s, ' ');
	if (vec[0] == "root" || vec[0] == "autoindex" || vec[0] == "upload")
		fill_root_autoindex_upload(loc, vec);
	else if (vec[0] == "cgiPath" || vec[0] == "cgiExt")
		fill_cgi_attr(loc, vec);
	else if (vec[0] == "index")
		fill_index(loc, vec);
	else if (vec[0] == "allowed_methods")
		fill_allowed_methods(loc, vec);
	else if (vec[0] == "return")
		fill_return(loc, vec);
	else
		fill_others(loc, vec);
	// else if (vec[0] == "error_pages")
	// 	fill_error_pages(loc, vec);
}

/* ######## PARSE ONE LOCATION ######## */
location	parse_one_location(std::string data)
{
	location	loc;
	std::vector<std::string> vec = str_split(data, '\n');
	
	loc.path = abstract_path(vec[0]);
	for (size_t i = 1; i < vec.size(); i++)
	{
		fill_location_attr(loc, vec[i]);
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
	if (!serv._locations.size())
		exit_mode("AT LEAST ONE LOCATION NEEDED IN SERVER BLOCK");
	if (serv._index.size() > 1)
		exit_mode("INDEX SHOULD HAS EXACTLY ONE ARGUMENT");
	return serv;
}

bool	is_server_block(std::string str, size_t pos)
{
	str = data_from_pos(str, pos + 6);
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isspace(str[i]))
		{
			if (str[i] != '{')
				exit_mode("AGAIN DAKHAL SERVER BLOCK M9AD AWELD NAS");
			return true;
		}
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

void	fill_meme_types(ConfigFile& config)
{
	std::string meme_types;

	meme_types = read_file("./configfile/MIME_TYPES");
	std::vector<std::string> list = str_split(meme_types, '\n');
	for (size_t i = 0; i < list.size(); i++)
	{
		std::vector<std::string> key_value = str_split(list[i], ' ');
		std::string key = key_value[0];
		std::string value = key_value[1];
		config._mime_types[key] = value;
		config._content_types.insert(value);
	}
}

std::string	str_join_status_def(std::vector<std::string> v)
{
	std::string res = "";

	for (size_t i = 1; i < v.size(); i++)
	{
		res += v[i];
		if (i < v.size() - 1)
			res += " ";
	}
	return res;
}

void	fill_code_status(ConfigFile& config)
{
	std::string code_status;

	code_status = read_file("configfile/code_status");
	std::vector<std::string> list = str_split(code_status, '\n');
	for (size_t i = 0; i < list.size(); i++)
	{
		std::vector<std::string> key_value = str_split(list[i], ' ');
		std::string key = key_value[0];
		std::string value = str_join_status_def(key_value);
		config._code_status[key] = value;
	}
	// for (auto &it : config._code_status)
	// {
	// 	std::cout << it.first << " " << it.second << std::endl;
	// }
}

ConfigFile	start_parse(std::string config_file)
{
	ConfigFile		conf;
	std::string		serv;

	conf._servers = parse_servers(config_file);
	errors_handling(conf._servers);
	// print_servers(conf._servers);
	fill_meme_types(conf);
	fill_code_status(conf);
	// exit(0);
	return (conf);
}