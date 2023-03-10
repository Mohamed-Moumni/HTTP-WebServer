/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/10 19:04:33 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket()
{
    
}

ConnectSocket::~ConnectSocket()
{
    
}

ConnectSocket::ConnectSocket(int _ConnectionId)
{
    ConnectSocketId = _ConnectionId;
    ReadAvailble = true;
    SendAvailble = false;
    Request = "";
}

void    ConnectSocket::get_content_length(std::string request)
{
    if (request.find("Content-Length") != std::string::npos)
    {
        ReadAvailble = true;
        SendAvailble = false;
        ContentLength = std::stoi(request.substr(request.find(" ", request.find("Content-Length")) + 1));
    }
    else
    {
        ReadAvailble = false;
        SendAvailble = true;
        ContentLength = 0;
    }
}

void    ConnectSocket::read_request(void)
{
    char *      buffer;
    int         readLength;
    static int  _contentLength;
    buffer = new char[BUFFER];

    if (ReadAvailble)
    {
        if (Request.empty())
        {
            readLength = recv(ConnectSocketId, buffer, BUFFER, 0);
            std::string str =  std::string(buffer, readLength);
            Request.append(str);
            get_content_length(Request);
        }
        else
        {
            if (!_contentLength)
                _contentLength += Request.substr(Request.find("\r\n\r\n") + 4).size();
            if (_contentLength < ContentLength)
            {
                readLength = recv(ConnectSocketId, buffer, BUFFER, 0);
                _contentLength += readLength;
                std::string str =  std::string(buffer, readLength);
                Request.append(str);
                if (_contentLength >= ContentLength)
                {
                    ReadAvailble = false;
                    SendAvailble = true;
                    _contentLength = 0;
                }
            }
        }
    }
    delete [] buffer;
}

void    ConnectSocket::send_response(std::string &response)
{
    static  unsigned long   CharSent;
    int                     SentLen;

    SentLen = 0;
    if (SendAvailble)
    {
        SentLen = send(ConnectSocketId, response.c_str() + CharSent, this->ResponseLength, 0);
        if (SentLen >= 0)
        {
            CharSent += SentLen;
            this->ResponseLength -= SentLen;
        }
        if (CharSent == response.size())
        {
            Request.clear();
            ContentLength = 0;
            this->ResponseLength = 0;
            SendAvailble = false;
            ReadAvailble = true;
            CharSent = 0;
        }
    }
}


void    ConnectSocket::setResponseLength(unsigned long  length)
{
    ResponseLength = length;
}

void    ConnectSocket::reset_data(void)
{
    Request.clear();
    ContentLength = 0;
    ResponseLength = 0;
    SendAvailble = false;
    ReadAvailble = true;
}