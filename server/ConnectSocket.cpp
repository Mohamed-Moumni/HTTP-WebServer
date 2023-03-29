/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/29 10:24:54 by mmoumni          ###   ########.fr       */
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
    _request.request_string = "";
    Chuncked = false;
    ReadFirst = false;
}

void    ConnectSocket::readRequest(void)
{
    char    Buffer[BUFFER];
    int     CharReaded;

    if (!ReadFirst)
    {
        CharReaded += recv(ConnectSocketId, Buffer, BUFFER, 0);
        _request.request_string.append(std::string(Buffer,CharReaded));
        // getContentLength();
        // if (Chuncked)
        // {
        // }
        // if (_request.ContentLen != 0)
        // {
        //     _request.bodyReaded += (CharReaded - _request.bodyStart);
        //     availablity();
        // }
        // else
        // {
        //     availablity();
        // }
        ReadFirst = true;
    }
    else
    {
        CharReaded += recv(ConnectSocketId, Buffer, BUFFER, 0);
        _request.request_string.append(std::string(Buffer, CharReaded));
        _request.ContentLen += CharReaded;
        availablity();
    }
}

void    ConnectSocket::availablity(void)
{
    if (_request.bodyReaded == _request.ContentLen)
    {
        ReadAvailble = false;
        SendAvailble = true;
    }
}


void    ConnectSocket::sendResponse(void)
{
    int CharSent;

    if (SendAvailble)
    {
        CharSent = send(ConnectSocketId, _response.response_string.c_str() + _response.CharSent, _response.ContentLength, 0);
        _response.CharSent += CharSent;
        _response.ContentLength -= CharSent;
        if (_response.ContentLength == 0)
        {
            SendAvailble = false;
            ReadAvailble = true;
            _response.response_string.clear();
            _response.ContentLength = 0;
            _response.CharSent = 0;
        }
    }
}

// void    ConnectSocket::getContentLength(void)
// {
//     std::map<std::string, std::string>::iterator contentLenHeader;
//     std::map<std::string, std::string>::iterator TeHeader;
//     TeHeader = _request.headers_map.find("Transfer-Encoding");
//     contentLenHeader = _request.headers_map.find("Content-Length");
//     if ( TeHeader != _request.headers_map.end() )
//     {
//         if (!(*TeHeader).second.compare("chunked"))
//             Chuncked = true;
//         else
//             Chuncked = false;
//     }
//     if ( contentLenHeader != _request.headers_map.end() )
//     {
//         ContentLength = std::stoi((*contentLenHeader).second);
//     }
// }