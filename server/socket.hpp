/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:52 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/04 20:58:51 by mmoumni          ###   ########.fr       */
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
typedef struct pollfd   pfd;
typedef struct sockaddr_storage sockStorage; 
typedef struct sockaddr         sockaddr;
class Socket
{
    private:
        std::string         Host;
        std::string         Port;
        int                 socketId;
        std::vector<Server> _servers;
    public:
        Socket();
        ~Socket();
        Socket(std::string hostname, std::string port);

        std::vector<int>    connectionId;
        int                 getSocketId(void);
        std::string         getHost(void);
        std::string         getPort(void);
        addrinfo            *getinfostruct(std::string hostname, std::string port);
        int                 createSocketId(addrinfo  *hints);
        // std::string recv_request(int sockId);
};

std::string recv_request(int sockId);
void        send_response(int sockId, std::string response);
#endif
