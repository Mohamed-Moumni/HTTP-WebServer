/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/29 08:39:00 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <map>
#include <vector>
#include <unistd.h>
#include "../request/request.class.hpp"
#include "../request/response.class.hpp"

#define BUFFER 1024

class ConnectSocket
{
    public:
        int             CharReaded;
        int             ConnectSocketId;
        bool            ReadAvailble;
        bool            SendAvailble;
        bool            persistance;
        bool            Chuncked;
        bool            ReadFirst;
        long long       TimeOut;
        size_t          ContentLength;
        std::string     IpAdress;
        std::string     Port;
        request         _request;
        response        _response;
        ~ConnectSocket();
        ConnectSocket();
        ConnectSocket(int SocketId, std::string _IpAdress, std::string _port);
        void    readRequest(void);
        void    readFirst(void);
        void    sendResponse(void);
        void    getContentLength(void);
};
