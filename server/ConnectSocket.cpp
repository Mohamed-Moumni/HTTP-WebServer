/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/04 14:29:56 by mmoumni          ###   ########.fr       */
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
            CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
            if (CharRead < 0)
            {
                std::cout << "recv: Error\n";
                exit(1);
            }
            _request.request_body.append(std::string(Buffer,CharRead));
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
        {
            HexToDec(_request.request_body, ChunckedSize);
            Chuncked = true;
        }
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
    // request_handler(*this, _configfile); new function
    requestType();
    if (Chuncked)
        readChuncked(Connections);
    else
    {
        _request.ContentLen -= CharRead;
        readUnChuncked(Connections);
    }
    _request.request_string.clear();
}

void    HexToDec(const std::string hexValue, size_t & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
}

void    ConnectSocket::readUnChuncked(std::map<int, ConnectSocket> & Connections)
{
    if (_request.ContentLen == -1)
    {
        ReadAvailble = false;
        SendAvailble = true;
        return ;
    }
    if (_request.ContentLen == 0)
    {
        ReadAvailble = false;
        SendAvailble = true;
    }
}

void    ConnectSocket::readChuncked(std::map<int, ConnectSocket> & Connections)
{
    std::string body;
    int         pos;

    pos = 0;
    pos = _request.request_body.find("\r\n");
    pos += 2;
    body = _request.request_body.substr(pos);
    _request.request_body.clear();
    _request.request_body.append(body, body.size());
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