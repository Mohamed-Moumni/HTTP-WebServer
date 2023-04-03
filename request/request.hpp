#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__


#include "../request/request.class.hpp"
#include "../request/response.class.hpp"
#include "../server/ConnectSocket.hpp"
#include <iostream>
#include <fstream>
#include <deque>
#include "../utils/utils.hpp"
#include "../configfile/configfile.hpp"
#include <dirent.h>
#include <string.h>
#include <unistd.h>

int request_handler(ConnectSocket & socket, ConfigFile configfile);

#endif