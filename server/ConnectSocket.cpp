/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/09 15:20:09 by mmoumni          ###   ########.fr       */
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
                    chunckBody(_configfile);
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
                readUnChuncked(_configfile);
            }
        }
    }
}

void    ConnectSocket::FirstRead(ConfigFile & _configfile)
{
    int         CharRead;
    char        Buffer[BUFFER];

    _request.BodyReaded = 0;
    _response.CharSent = 0;
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
        requestType(_configfile);
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

void    ConnectSocket::chunckBody(ConfigFile & _configfile)
{
    std::string body;

    try
    {
        body = getChuncked(_request.request_body);
    }
    catch(const std::exception& e)
    {
        closed = true;
        return ;
    }
    _request.request_body.clear();
    _request.request_body.append(body);
    respond(*this, _configfile);
    _response.respLength = _response.response_string.size();
    ReadAvailble = false;
    SendAvailble = true;
    ConnectionType();
}

void    ConnectSocket::requestType(ConfigFile & _configfile)
{
    std::map<std::string, std::string>::iterator Te;
    std::map<std::string, std::string>::iterator Cl;

    Te = _request.headers_map.find("Transfer-Encoding");
    Cl = _request.headers_map.find("Content-Length");
    if ( Te != _request.headers_map.end())
    {
        if (Te->second == "Chunked")
            Chuncked = true;
    }
    else if (Cl != _request.headers_map.end())
    {
        _request.ContentLen = atol(Cl->second.c_str());
        if (_request.ContentLen == _request.request_body.size())
        {
            ReadAvailble = false;
            SendAvailble = true;
            respond(*this, _configfile);
            _response.respLength = _response.response_string.size();
            ConnectionType();
        }
    }
    else
    {
        respond(*this, _configfile);
        _response.respLength = _response.response_string.size();
        ReadAvailble = false;
        SendAvailble = true;
        ConnectionType();
    }
}

void    HexToDec(const std::string hexValue, size_t & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
    if(ss.fail())
        throw std::runtime_error("chunk Error");
}

void    ConnectSocket::readUnChuncked(ConfigFile & _configfile)
{
    
    if (_request.ContentLen < _request.BodyReaded)
    {
        closed = true;
        return ;
    }
    if (_request.ContentLen == _request.BodyReaded)
    {
        ReadAvailble = false;
        SendAvailble = true;
        respond(*this, _configfile);
        _response.respLength = _response.response_string.size();
        ConnectionType();
    }
}

void    ConnectSocket::ConnectionType(void)
{
    std::map<std::string, std::string>::iterator connectType;

    connectType = _request.headers_map.find("Connection");
    if (connectType != _request.headers_map.end())
    {
        if (connectType->second == "close")
        {
            conType = true;
            return ;
        }
        else if (connectType->second == "keep-alive")
        {
            conType = false;
            return ;
        }
    }
    conType = true;
}

std::string ConnectSocket::getChuncked(std::string req)
{
    std::string body = "";
    std::string data;
    size_t pos;
    size_t  size = 1;

    pos = 0;
    while (size > 0)
    {
        HexToDec(&req[pos], size);
        pos = req.find("\r\n", pos);
        if (pos == std::string::npos)
            throw std::runtime_error("chunk Error");
        pos += 2;
        if (pos < req.size())
        {
            data = req.substr(pos, size);
            if (data.size() != size)
                throw std::runtime_error("chunk Error"); 
            body.append(data);
        }
        else
            throw std::runtime_error("chunk Error");
        pos = req.find("\r\n", pos);
        if (pos == std::string::npos)
            throw std::runtime_error("chunk Error");
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
    if (_response.respLength == _response.CharSent)
    {
        SendAvailble = false;
        ReadAvailble = true;
        _response.respLength = _response.response_string.size();
        _response.CharSent = 0;
    }
}