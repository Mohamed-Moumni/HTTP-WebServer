/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/26 13:38:42 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket()
{
    
}

ConnectSocket::~ConnectSocket()
{
    
}

ConnectSocket::ConnectSocket(int ConnectionId, std::string _IpAdress, std::string _Port)
{
    IpAdress = _IpAdress;
    Port = _Port;
    ConnectSocketId = ConnectionId;
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
    // int         readLength;
    // static int  _contentLength;

    buffer = new char[BUFFER];
    recv(ConnectSocketId, buffer, BUFFER, 0);
    std::cout << buffer << std::endl;
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

void    ConnectSocket::ConnectionType(std::string _request)
{
    if (_request.find("Connection: keep-alive") != std::string::npos)
        ConnectionClosed = false;
    else
        ConnectionClosed = true;
}
