/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/10 14:41:09 by mmoumni          ###   ########.fr       */
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
            FirstRead(_configfile);
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
    int         error;

    reqInit();
    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    // std::cout << Buffer << std::endl;
    if (CharRead <= 0)
    {
        // std::cout << "close1\n";
        closed = true;
        return ;
    }
    _request.request_string.append(std::string(Buffer, CharRead));
    if (_request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        error = request_handler(*this, _configfile);
        if (!error)
        {
            // std::cout << "close2\n";
            _response.response_string.append(respond_error("400", _configfile));
            _response.respLength = _response.response_string.size();
            closed = true;
            return ;
        }
        requestType(_configfile);
        _request.BodyReaded += _request.request_body.size();
        ReadFirst = true;
        _request.request_string.clear();
    }
    else
    {
        closed = true;        
        _response.response_string.append(respond_error("400", _configfile));
        _response.respLength = _response.response_string.size();
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
        _response.response_string.append(respond_error("400", _configfile));
        _response.respLength = _response.response_string.size();
        closed = true;
        return ;
    }
    _request.request_body.clear();
    _request.request_body.append(body);
    responding(_configfile);
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
        {
            Chuncked = true;
            if (_request.request_body.find("0\r\n\r\n") != std::string::npos)
                chunckBody(_configfile);
        }
    }
    else if (Cl != _request.headers_map.end())
    {
        _request.ContentLen = atol(Cl->second.c_str());
        if (_request.ContentLen == _request.request_body.size())
            responding(_configfile);
    }
    else
    {
        responding(_configfile);
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
        _response.response_string.append(respond_error("400", _configfile));
        _response.respLength = _response.response_string.size();
        closed = true;
        return ;
    }
    if (_request.ContentLen == _request.BodyReaded)
        responding(_configfile);
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
    size_t      pos;
    size_t      clrf;
    size_t      size = 1;

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
            clrf = req.find("\r\n", pos);
            if (clrf == std::string::npos || (clrf - pos) != size)
                throw std::runtime_error("chunk Error");
            data = req.substr(pos,size);
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

void        ConnectSocket::responding(ConfigFile & _configfile)
{
    ReadAvailble = false;
    SendAvailble = true;
    respond(*this, _configfile);
    _response.respLength = _response.response_string.size();
    ConnectionType();
}

void        ConnectSocket::reqInit(void)
{
    _request.BodyReaded = 0;
    _response.CharSent = 0;
    _response.respLength = 0;
    _request.request_string.clear();
    _response.response_string.clear();
}

void    ConnectSocket::clearData(void)
{
    _request.ContentLen = 0;
    _request.request_body.clear();
    _request.headers_map.clear();
    _request.request_target.clear();
    _request.http_version.clear();
    if (!conType)
        ReadFirst = false;
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
        clearData();
    }
}