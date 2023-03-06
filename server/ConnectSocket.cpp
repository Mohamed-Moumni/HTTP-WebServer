/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:31:00 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/06 16:46:43 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket()
{
    
}

ConnectSocket::~ConnectSocket()
{
    
}

ConnectSocket::ConnectSocket(int ConnectionId)
{
    ConnectSocketId = ConnectionId;
    readAvailble = true;
    sendAvailble = false;
    char_sent = 0;
}

void    ConnectSocket::get_content_length(std::string request)
{
    if (request.find("Content-Length") != std::string::npos)
    {
        ContentLengthAvailble = true;
        ContentLength = std::stoi(request.substr(request.find(" ", request.find("Content-Length")) + 1));
    }
    else
        ContentLengthAvailble = false;
}

void    ConnectSocket::read_request(void)
{
    char buffer[BUFFER];   
    if (readAvailble)
    {
        recv(ConnectSocketId, buffer, BUFFER, 0);
        this->readedRequest = std::string(buffer);
        // this->get_content_length(this->readedRequest);
        // if (ContentLengthAvailble)
        // {
        //     ContentLength -= BUFFER;
        // }
        // else
        // {
        //     readAvailble = false;
        // }
        std::cout << readedRequest << std::endl;
        sendAvailble = true;
        readAvailble = false;
    }
}

void    ConnectSocket::send_response(std::string response)
{
    const char  *buffer;
    buffer = response.c_str();
    if (sendAvailble && char_sent <= response_Length)
    {
        send(ConnectSocketId, buffer, strlen(buffer), 0);
        sendAvailble = false;
        readAvailble = true;
    }
}
