/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/14 10:24:21 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "socket.hpp"

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
    {
        std::cout << "getaddrinfo Error: " << gai_strerror(err) << std::endl;
        exit(EXIT_FAILURE);
    }
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
        {
            std::cout << "fcntl Error: " << std::endl;
        }
        if ((setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(var))) < 0)
        {
            std::cout << "setsockopt Error: " << std::endl;
            continue;
        }
        if ((bind(sockId, p->ai_addr, p->ai_addrlen)) < 0)
        {
            std::cout << strerror(errno) << std::endl;
        }
        else
            break ;
        close(sockId);
    }
    freeaddrinfo(hints);
    if (p == NULL)
    {
        std::cout << "Can't create the socket: invalid Ip address or port" << std::endl;
        exit(EXIT_FAILURE);
    }
    return (sockId);
}

std::vector<Socket> CreateSockets(const std::vector<Server> & _server)
{
    size_t              ServerCount;
    std::vector<Socket> sockets;

    ServerCount = _server.size();
    for (size_t i = 0; i < ServerCount; i++)
    {
                   
    }
}