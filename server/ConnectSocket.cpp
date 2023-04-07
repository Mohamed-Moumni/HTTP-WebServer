/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/07 15:45:13 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"
#include "../server/ConnectSocket.hpp"
#include "../server/socket.hpp"
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
    closed = false;
    conType = false;
    chunck = false;
}

void    ConnectSocket::readRequest(ConfigFile & _configfile)
{
    int     CharRead;
    char    Buffer[BUFFER];

    CharRead = 0;
    if (ReadAvailble)
    {
        if (!ReadFirst)
        {
            FirstRead(_configfile);
        }
        else
        {
            if (Chuncked)
            {
                readingChuncked();
                if (chunck)
                    chunckBody();
            }
            else
            {
                if (_request.ContentLen == std::string::npos)
                {
                    ReadAvailble = false;
                    SendAvailble = true;
                    ConnectionType();
                }
                else
                {
                    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
                    if (CharRead <= 0)
                    {
                        closed = true;
                        return ;
                    }
                    _request.request_body.append(std::string(Buffer, CharRead));
                    _request.BodyReaded += CharRead;
                    readUnChuncked();
                }
            }
        }
    }
}

void    ConnectSocket::FirstRead(ConfigFile & _configfile)
{
    int         CharRead;
    char        Buffer[BUFFER];
    std::string body;

    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    if (CharRead <= 0)
    {
        closed = true;
        return ;
    }
    _request.request_string.append(std::string(Buffer, CharRead));
    if (_request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        request_handler(*this, _configfile);
        requestType();
        _request.BodyReaded += _request.request_body.size();
        ReadFirst = true;
        _request.request_string.clear();
    }
}

void    ConnectSocket::readingChuncked(void)
{
    int         CharRead;
    char        Buffer[BUFFER];

    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    if (CharRead <= 0)
    {
        closed = true;
        return ;
    }
    _request.request_body.append(std::string(Buffer, CharRead));
    if (_request.request_body.find("0\r\n\r\n") != std::string::npos)
        chunck = true;
}

void    ConnectSocket::chunckBody(void)
{
    std::string body;

    body = getChuncked(_request.request_body);
    _request.request_body.clear();
    _request.request_body.append(body);
    std::cout << _request.request_body << std::endl;
    ReadAvailble = false;
    SendAvailble = true;
    ConnectionType();
}

void        ConnectSocket::requestType(void)
{
    std::map<std::string , std::string>::iterator Te;
    std::map<std::string , std::string>::iterator Cl;

    Te = _request.headers_map.find("Transfer-Encoding");
    Cl = _request.headers_map.find("Content-Length");
    if ( Te != _request.headers_map.end())
    {
        if (Te->second == "Chunked")
        {
            Chuncked = true;
        }
    }
    else if (Cl != _request.headers_map.end())
    {
        _request.ContentLen = atol(Cl->second.c_str());
    }
    else
    {
        _request.ContentLen = std::string::npos;
    }
}


void    HexToDec(const std::string hexValue, size_t & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
}

void    ConnectSocket::readUnChuncked(void)
{
    if (_request.ContentLen == _request.BodyReaded)
    {
        ReadAvailble = false;
        SendAvailble = true;
        ConnectionType();
    }
}

void    ConnectSocket::ConnectionType(void)
{
    std::map<std::string , std::string>::iterator connectType;

    connectType = _request.headers_map.find("Connection");
    if (connectType != _request.headers_map.end())
    {
        if (connectType->second == "close")
            conType = true;
    }
}

std::string ConnectSocket::getChuncked(std::string req)
{
    std::string body = "";
    size_t pos;
    size_t  size = 1;

    pos = 0;
    while (size > 0)
    {
        HexToDec(&req[pos], size);
        pos = req.find("\r\n", pos);
        if (pos == std::string::npos)
            break ;
        pos += 2;
        if (pos < req.size())
            body.append(req.substr(pos, size));
        pos = req.find("\r\n", pos);
        if (pos == std::string::npos)
            break ;
        pos += 2;
    }
    return (body);
}

void    ConnectSocket::sendResponse(void)
{
    int CharSent;

    CharSent = 0;
    CharSent = send(ConnectSocketId, _response.response_string.c_str() + _response.CharSent, _response.respLength, 0);
    if (CharSent <= 0)
    {
        closed = true;
        return ;
    }
    _response.CharSent += CharSent;
    _response.respLength -= CharSent;
    if (_response.respLength == 0)
    {
        SendAvailble = false;
        ReadAvailble = true;
        _response.respLength = _response.response_string.size();
        _response.CharSent = 0;
        ReadFirst = false;
    }
}