/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/02 13:19:36 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"
#include "../server/ConnectSocket.hpp"
#include "../configfile/configfile.hpp"
#include "../request/INCLUDES/request.class.hpp"
#include "../request/INCLUDES/request.hpp"

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

// void    ConnectSocket::readRequest(ConfigFile & _configfile)
// {
//     // char    Buffer[BUFFER];
//     int     CharReaded;

//     (void)(_configfile);
//     CharReaded = 0;
//     if (ReadAvailble)
//     {
//         if (!ReadFirst)
//         {
//             FirstRead(_configfile);
//         }
//         else
//         {
//             if (this->Chuncked)
//                 // readChuncked();
//             else    
//                 readContentLength();
//             if (_request.ContentLen == 0)
//             {
//                 ReadAvailble = false;
//                 SendAvailble = true;
//             }
//         }
//     }
// }

// void    ConnectSocket::FirstRead(ConfigFile & _configfile)
// {
//     int     CharRead;
//     char    Buffer[BUFFER];

//     CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
//     _request.request_string.append(std::string(Buffer, CharRead));
//     request_handler(*this, _configfile);
//     if (Chuncked)
//     {
//         readChuncked()
//     }
//     else
//     {
        
//     }
// }

size_t  hex2dec(std::string hex)
{
    size_t  result = 0;
    for (size_t i=0; (hex[i] != '\r' && hex[i] != '\n'); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

std::string ConnectSocket::getChunckedbody(std::string _req)
{
    std::string body;
    std::string data;
    size_t      size;
    size_t      pos;

    body = "";
    pos = 0;
    size = hex2dec(&_req[pos]);
    pos = _req.find("\r\n", pos) + 2;
    data = _req.substr(pos, size);
    while (size > 0)
    {
        body.append(std::string(data, size));
        pos = _req.find("\r\n", pos) + 2; 
        size = hex2dec(&_req[pos]);
        pos = _req.find("\r\n", pos) + 2;
        data = _req.substr(pos, size);
    }
    return (body);
}

void    ConnectSocket::readChuncked(void)
{
    
}

// void    ConnectSocket::readContentLength(void)
// {
//     int CharRead;
//     char    Buffer[BUFFER];

//     CharRead = 0;
//     CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
//     _request.ContentLen -= CharRead;
//     _request.request_string.append(std::string(Buffer, CharRead));
// }

long long   getTimeOfnow(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return(time.tv_sec + time.tv_usec / 1000000);
}

// void    ConnectSocket::availablity(void)
// {
//         ReadAvailble = false;
//         SendAvailble = true;
//     }
// }

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