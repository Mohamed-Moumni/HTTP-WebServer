/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/25 08:17:07 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "socket.hpp"
#include "../configfile/configfile.hpp"
#include "../configfile/server.hpp"

Socket::Socket()
{
    
}

Socket::~Socket()
{
       
}

Socket::Socket(std::string hostname, std::string port)
{
    addrinfo    *hints;

    Host = hostname;
    Port = port;
    hints = this->getinfostruct(hostname, port);
    socketId = createSocketId(hints);
}

int Socket::getSocketId(void)
{
    return (this->socketId);
}

std::string Socket::getHost(void)
{
    return (this->Host);
}

std::string Socket::getPort(void)
{
    return (this->Port);
}

addrinfo    *Socket::getinfostruct(std::string hostname, std::string port)
{
    addrinfo hints, *res;
    int err;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &res)) < 0)
        throw std::runtime_error("getaddrinfo Error: " + std::string(gai_strerror(err)));
    return (res);
}

int Socket::createSocketId(addrinfo  *hints)
{
    addrinfo    *p;
    int         sockId, var;

    p = hints;
    var = 1;
    for (;p != NULL; p = p->ai_next)
    {
        sockId = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockId == -1)
            continue;
        if (fcntl(sockId, F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("fcntl Error: ");
        if ((setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(var))) < 0)
            continue;
        if ((bind(sockId, p->ai_addr, p->ai_addrlen)) < 0)
            throw std::runtime_error(std::string(strerror(errno)));
        else
            break ;
        close(sockId);
    }
    freeaddrinfo(hints);
    if (p == NULL)
        throw std::runtime_error("Can't create the socket: invalid Ip address or port");
    return (sockId);
}

void    listenSocket(std::vector<Socket> & _sockets)
{
    for (size_t i = 0; i < _sockets.size(); i++)
    {
        if (listen(_sockets[i].getSocketId(), 50) < 0)
        {
            std::cout << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::vector<pfd>    create_pfd(std::vector<Socket> & _sockets)
{
    std::vector<pfd>    pfds;
    pfd                 temp_pfd;

    for (size_t i = 0; i < _sockets.size(); i++)
    {
        temp_pfd.fd = _sockets[i].getSocketId();
        temp_pfd.events = POLLIN | POLLOUT;
        pfds.push_back(temp_pfd);
    }
    return (pfds);
}

std::vector<Socket> create_sockets(ConfigFile & _configfile)
{
    std::vector<Socket> _sockets;
    std::map<std::string, std::vector<std::string> >::iterator listenIter;

    for (size_t i = 0; i < _configfile._servers.size(); i++)
    {
        listenIter = _configfile._servers[i]._listen.begin();
        for (; listenIter != _configfile._servers[i]._listen.end(); listenIter++)
        {
            for (size_t j = 0; j < listenIter->second.size(); j++)
            {
                Socket s(listenIter->first, listenIter->second[j]);
                _sockets.push_back(s);   
            }
        }
    }
    return (_sockets);
}