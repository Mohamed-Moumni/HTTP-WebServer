/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/10 15:58:00 by mmoumni          ###   ########.fr       */
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
    std::cout << "ReadFirst\n";
    CharRead = recv(ConnectSocketId, Buffer, BUFFER, 0);
    if (CharRead <= 0)
    {
        closed = true;
        return ;
    }
    _request.request_string.append(std::string(Buffer, CharRead));
    if (_request.request_string.find("\r\n\r\n") != std::string::npos)
    {
        error = request_handler(*this, _configfile);
        if (!error)
        {
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
        _response.response_string.append(respond_error("413", _configfile));
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

void        ConnectSocket::responding(ConfigFile & _configfile)
{
    ReadAvailble = false;
    SendAvailble = true;
    respond(*this, _configfile);
    // std::cout << "the response is : +++++++++++\n" << _response.response_string << "\n++++++++++++\n" ; 
    _response.respLength = _response.response_string.size();
    ConnectionType();
}

void    ConnectSocket::sendResponse(void)
{
    int CharSent;

    CharSent = 0;
    // std::cout << "before send: +++++++++++\n" << _response.response_string << "++++++++++"<< std::endl;
    CharSent = send(ConnectSocketId, _response.response_string.c_str() + _response.CharSent, _response.respLength, 0);
    // std::cout << "after send: +++++++++++\n" << _response.response_string << "++++++++++"<< std::endl;
    if (CharSent <= 0)
    {
        closed = true;
        return ;
    }
    _response.CharSent += CharSent;
    // std::cout << _response.response_string << "---" << CharSent << "----" << _response.respLength << "\n";
    if (_response.respLength == _response.CharSent)
    {
        SendAvailble = false;
        ReadAvailble = true;
        clearData();
    }
}