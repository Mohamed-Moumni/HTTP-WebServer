/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/31 14:01:22 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"
#include "../server/ConnectSocket.hpp"
#include "../configfile/configfile.hpp"
#include "../request/request.class.hpp"
#include "../request/request.hpp"

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
    ReadAvailble = true;
    SendAvailble = false;
    Chuncked = false;
    ReadFirst = false;
}

void    ConnectSocket::readRequest(ConfigFile & _configfile)
{
    char    Buffer[BUFFER];
    int     CharReaded;

    (void)(_configfile);
    CharReaded = 0;
    if (ReadAvailble)
    {
        if (!ReadFirst)
        {
            CharReaded += recv(ConnectSocketId, Buffer, BUFFER, 0);
            _request.request_string.append(std::string(Buffer,CharReaded));
                ReadAvailble = false;
                SendAvailble = true;
            // request_handler(*this, _configfile);
        }
        else
        {
            // if (this->Chuncked)
            // {
            //     readChuncked();
            // }
            // else
            // {
            //     readContentLength();
            // }
            // if (_request.ContentLen == _request.bodyReaded)
            // {
            // }
        }
    }
}

// std::string ConnectSocket::getChunckedbody(void)
// {
//     std::string body;
//     size_t      contentLength;

//     body = "";
//     return 
// }

void    ConnectSocket::readChuncked(void)
{
       
}

void    ConnectSocket::readContentLength(void)
{
    
}

long long   getTimeOfnow(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return(time.tv_sec + time.tv_usec / 1000000);
}

void    ConnectSocket::availablity(void)
{
    if (_request.bodyReaded == _request.ContentLen)
    {
        ReadAvailble = false;
        SendAvailble = true;
    }
}

void    ConnectSocket::sendResponse( void )
{
    int CharSent;

    CharSent = 0;
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