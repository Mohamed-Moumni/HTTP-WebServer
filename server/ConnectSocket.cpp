/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/29 09:02:31 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket()
{
    
}

ConnectSocket::~ConnectSocket()
{
    
}


ConnectSocket::ConnectSocket(int SocketId, std::string _IpAdress, std::string _port)
{
    ConnectSocketId = SocketId;
    IpAdress = _IpAdress;
    Port = _port;
    CharReaded = 0;
    _request.request_string = "";
    Chuncked = false;
    ReadFirst = false;
}

void    ConnectSocket::readRequest(void)
{
    char    Buffer[BUFFER];

    if (!ReadFirst)
    {
        CharReaded += recv(ConnectSocketId, Buffer, BUFFER, 0);
        _request.request_string.append(std::string(Buffer,CharReaded));
        getContentLength();
        if (Chuncked)
        {
            
        }
        else
        {
            
        }
    }
    else
    {
        
    }
}

void    ConnectSocket::sendResponse(void)
{
       
}

void    ConnectSocket::getContentLength(void)
{
    std::map<std::string, std::string>::iterator contentLenHeader;
    std::map<std::string, std::string>::iterator TeHeader;
    TeHeader = _request.headers_map.find("Transfer-Encoding");
    contentLenHeader = _request.headers_map.find("Content-Length");
    if ( TeHeader != _request.headers_map.end() )
    {
        if (!(*TeHeader).second.compare("chunked"))
            Chuncked = true;
        else
            Chuncked = false;
    }
    if ( contentLenHeader != _request.headers_map.end() )
    {
        ContentLength = std::stoi((*contentLenHeader).second);
    }
}