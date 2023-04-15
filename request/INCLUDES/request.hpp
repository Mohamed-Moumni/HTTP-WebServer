#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#define CRLF "\r\n"

#include "../../server/ConnectSocket.hpp"
#include "../../server/socket.hpp"
#include "../../utils/utils.hpp" 

/////////////PARSING RE////////////
std::string		read_file(std::string file_name);
int request_handler(ConnectSocket & socket, ConfigFile configfile);
int pars_request(request &request);
int possible_error(ConnectSocket &socket, ConfigFile configfile);

/////////////RESPONSE////////////
int respond(ConnectSocket &socket, ConfigFile configfile);
void response_generator(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile);
std::string respond_error(std::string error, ConfigFile configfile);
void file2response(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile);
int isdirectory(std::string path);
void redirect(ConnectSocket &socket,location location, Server server, ConfigFile configfile);
void cgi_handler(ConnectSocket &socket, location location,Server server, ConfigFile configfile);
std::string                 get_extention(std::string str);

/////////////METHODS/////////////
void GET(ConnectSocket &socket, Server &server, location &location, ConfigFile configfile);
void POST(ConnectSocket &socket, Server server, location location, ConfigFile configfile);
void DELETE(ConnectSocket &socket, Server server, location location, ConfigFile configfile);

#endif