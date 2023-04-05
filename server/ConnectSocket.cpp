/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/05 16:04:32 by mmoumni          ###   ########.fr       */
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
    ConnectionType = false;
    IpAdress = _IpAdress;
    Port = _port;
    _request.request_string = "";
    ReadAvailble = true;
    SendAvailble = false;
    Chuncked = false;
    ReadFirst = false;
    closed = false;
    sended = false;
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
            CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
            if (!CharRead)
            {
                // std::cout << "recv: Error\n";
                closed = true;
                return ;
            }
            _request.request_body.append(std::string(Buffer,CharRead));
            if (Chuncked)
                readChuncked();
            else
            {
                _request.ContentLen -= CharRead;   
                readUnChuncked();
            }
            _request.request_string.clear();
        }
    }
}

void    ConnectSocket::FirstRead(ConfigFile & _configfile)
{
    int         CharRead;
    char        Buffer[BUFFER];
    std::string body;

    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    if (!CharRead)
    {
        // std::cout << "recv: Error\n";
        closed = true;
        return ;
    }
    _request.request_string.append(std::string(Buffer, CharRead));
    request_handler(*this, _configfile);
    requestType();
    if (Chuncked)
    {
        readChuncked();
    }
    else
    {
        if (_request.ContentLen)
            _request.ContentLen -= _request.request_body.size();
        // std::cout << _request.ContentLen << std::endl;
        readUnChuncked();
    }
    _request.request_string.clear();
    ReadFirst = true;
}

void    ConnectSocket::readChuncked(void)
{
    std::string body;

    body = "";
    if (_request.request_body.find("0\r\n\r\n") == std::string::npos)
    {
        body.append(_request.request_body, _request.request_body.size());
        _request.request_body.clear();
        _request.request_body.append(getChuncked(body));
        ReadAvailble = false;
        SendAvailble = true;
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
    if (_request.ContentLen == 0 || _request.ContentLen == std::string::npos)
    {
        ReadAvailble = false;
        SendAvailble = true;
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
    // std::cout << _response.response_string << std::endl;
    CharSent = send(ConnectSocketId, _response.response_string.c_str() + _response.CharSent, _response.respLength, 0);
    // std::cout << CharSent << std::endl;
    _response.CharSent += CharSent;
    _response.respLength -= CharSent;
    if (_response.respLength == 0)
    {
        SendAvailble = false;
        ReadAvailble = true;
        _response.respLength = _response.response_string.size();
        _response.CharSent = 0;
        ReadFirst = false;
        sended = true;
    }
}