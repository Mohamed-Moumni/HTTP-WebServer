/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/05 13:23:58 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "socket.hpp"
#include "ConnectSocket.hpp"
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
    std::string host;
    std::vector<Socket> _sockets;
    std::set<std::pair<std::string, std::string> > processedIp;
    std::map<std::string, std::set<std::string> >::iterator listenIter;

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
                if (processedIp.find(std::make_pair(host, *setIter)) == processedIp.end())
                {
                    Socket s(host, *setIter);
                    _sockets.push_back(s);
                    processedIp.insert(std::make_pair(host, *setIter));
                }
            }
        }
    }
    return (_sockets);
}

void    pollin(std::vector<pfd> & pfds, std::vector<Socket> & _sockets, std::map<int, ConnectSocket> & Connections, size_t i)
{
    int connection;
    pfd tmp_pfd;
    
    connection = accept(pfds[i].fd, NULL, NULL);
    tmp_pfd.fd = connection;
    tmp_pfd.events = (POLLIN | POLLOUT);
    pfds.push_back(tmp_pfd);
    Connections[connection] = ConnectSocket(connection, _sockets[i].getHost(), _sockets[i].getPort());
    Connections[connection]._response.response_string = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 22\r\n\r\nHelloWorld\r\nHelloWorld\r\n";
    Connections[connection]._response.respLength = Connections[connection]._response.response_string.size();
    Connections[connection]._response.CharSent = 0;
}

void    pollout(ConfigFile & _configfile, std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i)
{
    if (Connections.find(pfds[i].fd) != Connections.end() && Connections[pfds[i].fd].SendAvailble)
    {
        Connections[pfds[i].fd].sendResponse();
        if (Connections[pfds[i].fd].ConnectionType)
            closeConnection(pfds, Connections, i);
    }
}

void    pollErrHup(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i)
{
    closeConnection(pfds, Connections, i);
}

void    closeConnection(std::vector<pfd> & pfds, std::map<int, ConnectSocket> & Connections, size_t i)
{
    close(pfds[i].fd);
    Connections.erase(pfds[i].fd);
    pfds.erase(pfds.begin() + i);
}

// void    sendError(int fd, std::vector<pfd> &pfds, std::map<int, ConnectSocket> & connections, ConfigFile & _configfile, std::string _Error)
// {
//     std::map<std::string, std::string>::iterator error = _configfile._servers[0]._error_pages.find(_Error);
//     send(fd, error->second.c_str(), error->second.size(), 0);
//     close(fd);
//     connections.erase(fd);
//     pfds.erase(pfds.begin() + i);
// }