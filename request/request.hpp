#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include "../configfile/configfile.hpp"
#include "../request/request.class.hpp"
#include "../request/response.class.hpp"
#include "../server/ConnectSocket.hpp"

int request_handler(ConnectSocket & socket, ConfigFile configfile);

#endif