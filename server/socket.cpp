/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/26 13:38:35 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "socket.hpp"
#include "../configfile/configfile.hpp"
#include "../configfile/server.hpp"
#include "ConnectSocket.hpp"

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
    std::map<std::string, std::set<std::string> >::iterator listenIter;
    std::string host;

    for (size_t i = 0; i < _configfile._servers.size(); i++)
    {
        listenIter = _configfile._servers[i]._listen.begin();
        for (; listenIter != _configfile._servers[i]._listen.end(); listenIter++)
        {
            host = listenIter->first;
            std::set<std::string>::iterator setIter;

            setIter = listenIter->second.begin();
            for (; setIter != listenIter->second.end(); setIter++)
            {
                Socket s(host, *setIter);
                std::cout << "listening on host: " << host << " port: " << *setIter << std::endl;
                _sockets.push_back(s);
            }
        }
    }
    return (_sockets);
}

void                pollin(std::vector<pfd> & pfds, std::vector<Socket> & _sockets, std::map<int, ConnectSocket> & Connections, size_t i)
{
    int         connection;
    sockStorage so_storage;
    socklen_t   socket_len;
    pfd         temp_pfd;

    if (i < _sockets.size() && pfds[i].fd == _sockets[i].getSocketId())
    {
        connection = accept(pfds[i].fd, (sockaddr *)&so_storage, &socket_len);
        temp_pfd.fd = connection;
        temp_pfd.events = POLLIN | POLLOUT;
        Connections[connection] = ConnectSocket(connection, _sockets[i].getHost(), _sockets[i].getPort());
    }
    else
    {
        if (Connections.find(pfds[i].fd) != Connections.end() && Connections[pfds[i].fd].ReadAvailble)
        {
            Connections[pfds[i].fd].read_request();
        }
    }
}

void                pollout(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i)
{
    std::string respo = "HTTP/1.1 200 OK\r\nContent-Type: text/plain-text\r\nConnection: Closed\r\n\r\nHello World";
    if (Connections.find(pfds[i].fd) != Connections.end())
    {
        Connections[pfds[i].fd].send_response(respo);
    }
}
