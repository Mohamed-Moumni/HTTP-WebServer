#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include "../configfile/configfile.hpp"
#include "../request/request.class.hpp"
#include "../request/response.class.hpp"

int request_handler(request &request, response &response, ConfigFile configfile);

#endif