/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocketUtils.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:52:11 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/15 16:51:40 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"
#include "../server/ConnectSocket.hpp"
#include "../server/socket.hpp"
#include "../configfile/configfile.hpp"
#include "../request/INCLUDES/request.class.hpp"
#include "../request/INCLUDES/request.hpp"

void    ConnectSocket::clearData(void)
{
    _response.CharSent = 0;
    _response.respLength = 0;
    _response.response_string.clear();
    _request.BodyReaded = 0;
    _request.ContentLen = 0;
    _request.request_string.clear();
    _request.request_body.clear();
    _request.headers_map.clear();
    _request.request_target.clear();
    _request.http_version.clear();
    if (!conType)
        ReadFirst = false;
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

void    HexToDec(const std::string hexValue, size_t & result)
{
    std::stringstream ss;
    ss << std::hex << hexValue;
    ss >> result;
    if(ss.fail())
        throw std::runtime_error("chunk Error");
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
        if (_request.ContentLen == _request.BodyReaded)
        {
            responding(_configfile);
        }
    }
    else
        responding(_configfile);
}