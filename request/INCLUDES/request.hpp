#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__


#include "../../server/ConnectSocket.hpp"
#include "../../utils/utils.hpp"

/////////////PARSING RE////////////
int request_handler(ConnectSocket & socket, ConfigFile configfile);
int pars_request(request &request);
std::string respond_error(std::string error);

/////////////RESPONSE////////////
int respond(ConnectSocket &socket, ConfigFile configfile);
void response_generator(ConnectSocket &socket, Server &server, location &location);

/////////////METHODS/////////////
void GET(ConnectSocket &socket, Server &server, location &location);
void POST();
void DELETE();

#endif