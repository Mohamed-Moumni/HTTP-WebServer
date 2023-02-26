/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:52 by mmoumni           #+#    #+#             */
/*   Updated: 2023/02/26 16:32:31 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <vector>
#include <errno.h>
#include "../configfile/server.hpp"

typedef struct addrinfo addrinfo;

class Socket
{
    private:
        std::string         Host;
        std::string         Port;
        int                 socketId;
        std::vector<Server> _servers;
        std::vector<int>    connectionId;
    public:
        Socket(std::string hostname, std::string port);
        Socket();
        ~Socket();
        addrinfo    *getinfostruct(std::string hostname, std::string port);
        int         getSockId(addrinfo  *hints);
};

#endif
