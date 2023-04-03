/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/03 15:05:50 by mmoumni          ###   ########.fr       */
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

void    ConnectSocket::readUnChuncked(std::map<int, ConnectSocket> & Connections)
{
    int     CharRead;
    char    Buffer[BUFFER];

    CharRead = 0;
    if (_request.ContentLen == -1)
    {
        ReadAvailble = false;
        SendAvailble = true;
        return ;
    }
    _request.ContentLen -= CharRead;
    if (_request.ContentLen == 0)
    {
        ReadAvailble = false;
        SendAvailble = true;
    }
}

void    ConnectSocket::readRequest(ConfigFile & _configfile, std::map<int, ConnectSocket> & Connections)
{
    int     CharRead;
    char    Buffer[BUFFER];

    CharRead = 0;
    if (ReadAvailble)
    {
        if (!ReadFirst)
        {
            FirstRead(_configfile, Connections);
        }
        else
        {
            _request.request_string.append(std::string(Buffer,CharRead));
            if (Chuncked)
                readChuncked(Connections);
            else
                readUnChuncked(Connections);
            _request.request_string.clear();
        }
    }
}

void        ConnectSocket::requestType(void)
{
    std::map<std::string , std::string>::iterator Te;
    std::map<std::string , std::string>::iterator Cl;
    Te = _request.headers_map.find("Transfer-Encoding");
    Cl = _request.headers_map.find("Content-Length");
    if ( Te != _request.headers_map.end())
    {
        if (Te->second == "Chuncked")
            Chuncked = true;
    }
    else if (Cl != _request.headers_map.end())
    {
        _request.ContentLen = atol(Cl->second.c_str());
    }
    else
    {
        _request.ContentLen = -1;
    }
}

void    ConnectSocket::FirstRead(ConfigFile & _configfile, std::map<int, ConnectSocket> & Connections)
{
    int         CharRead;
    char        Buffer[BUFFER];
    std::string body;

    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    if (CharRead < 0)
    {
        std::cout << "recv: Error\n";
        exit(1);
    }
    _request.request_string.append(std::string(Buffer, CharRead));
    // request_handler(*this, _configfile);
    requestType();
    if (Chuncked)
    {
        body = getChunckedbody(_request.request_body, Connections);
        _request.request_body.clear();
        _request.request_body = body;
    }
    else
        readUnChuncked(Connections);
    _request.request_string.clear();
}

size_t  hex2dec(std::string hex)
{
    size_t  result;

    result = 0;
    for (size_t i = 0; i < hex.size() && hex[i] && hex[i] != '\r' && hex[i] != '\n' ; i++)
    {
        if (hex[i] >= 48 && hex[i] <= 57)
        {
            result += (hex[i] - 48) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70)
        {
            result += (hex[i] - 55) * pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102)
        {
            result += (hex[i] - 87) * pow(16, hex.length() - i - 1);
        }
        else
            return (-1);
    }
    return result;
}

std::string ConnectSocket::getChunckedbody(std::string _req, std::map<int, ConnectSocket> & Connections)
{
    std::string body;
    std::string data;
    size_t      size;
    size_t      pos;

    body = "";
    pos = 0;
    size = hex2dec(&_req[pos]);
    if (size == 0)
    {
        ReadAvailble = false;
        SendAvailble = true;
        return (body);
    }
    pos = _req.find("\r\n", pos);
    if (pos == std::string::npos)
        return (body);
    pos += 2;
    data = _req.substr(pos , size);
    while (size > 0)
    {
        body.append(std::string(data, size));
        pos = _req.find("\r\n", pos);
        if (pos == std::string::npos)
            return (body);
        pos += 2;
        size = hex2dec(&_req[pos]);
        if (size == 0)
        {
            ReadAvailble = false;
            SendAvailble = true;
            return (body);
        }
        pos = _req.find("\r\n", pos);
        if (pos == std::string::npos)
            return (body);
        pos += 2;
        data = _req.substr(pos, size);
    }
    return (body);
}

void    ConnectSocket::readChuncked(std::map<int, ConnectSocket> & Connections)
{
    std::string body;

    body.append(getChunckedbody(_request.request_string, Connections));
    _request.request_body.clear();
    _request.request_body = body;
}

void    ConnectSocket::sendResponse(std::map<int, ConnectSocket> & Connections)
{
    int CharSent;

    CharSent = 0;
    CharSent = send(ConnectSocketId, _response.response_string.c_str() + _response.CharSent, _response.respLength, 0);
    _response.CharSent += CharSent;
    _response.respLength -= CharSent;
    if (_response.respLength == 0)
    {
        SendAvailble = false;
        ReadAvailble = true;
        _response.response_string.clear();
        _response.respLength = 0;
        _response.CharSent = 0;
    }
}