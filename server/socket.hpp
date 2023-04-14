/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:52 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/13 14:29:47 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include "../configfile/configfile.hpp"
#include "./ConnectSocket.hpp"

typedef struct addrinfo         addrinfo;
typedef struct pollfd           pfd;

class Socket
{
    private:
        std::string         Host;
        std::string         Port;
        int                 socketId;
    public:
        Socket();
        ~Socket();
        Socket(std::string hostname, std::string port);

        int                 getSocketId(void);
        std::string         getHost(void);
        std::string         getPort(void);
        addrinfo            *getinfostruct(std::string hostname, std::string port);
        int                 createSocketId(addrinfo  *hints);
};

void                listenSocket(std::vector<Socket> & _sockets);
std::vector<Socket> create_sockets(ConfigFile & _configfile);
std::vector<pfd>    create_pfd(std::vector<Socket> & _sockets);
void                pollin(std::vector<pfd> & pfds, std::vector<Socket> & _sockets, std::map<int, ConnectSocket> & Connections, size_t i);
void                pollout(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i);
void                pollErrHup(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i);
void                closeConnection(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i);
void                acceptConnection(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections);
void                sendError(int socketId, std::string _Error);
long long           getTimeOfNow(void);
void                checkTimeOut(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, ConfigFile & _configfile, size_t i);