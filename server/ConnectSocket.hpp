/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/04 14:15:40 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <math.h>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/time.h>
#include "../request/INCLUDES/request.class.hpp"
#include "../request/INCLUDES/response.class.hpp"
#include "../configfile/configfile.hpp"

#define BUFFER 1024

class ConnectSocket
{
    public:
        int             ConnectSocketId;
        bool            ReadAvailble;
        bool            SendAvailble;
        bool            ConnectionType;
        bool            Chuncked;
        bool            ReadFirst;
        size_t          ChunckedSize;
        long long       TimeOut;
        std::string     IpAdress;
        std::string     Port;
        request         _request;
        response        _response;
        ~ConnectSocket();
        ConnectSocket();
        ConnectSocket(int SocketId, std::string _IpAdress, std::string _port);
        void        readRequest(ConfigFile & _configfile, std::map<int, ConnectSocket> & Connections);
        void        readFirst(void);
        void        sendResponse(std::map<int, ConnectSocket> & Connections);
        void        getContentLength(void);
        void        readChuncked(std::map<int, ConnectSocket> & Connections);
        void        readUnChuncked(std::map<int, ConnectSocket> & Connections);
        void        FirstRead(ConfigFile & _configfile, std::map<int, ConnectSocket> & Connections);
        std::string getChunckedbody(std::string _req, std::map<int, ConnectSocket> & Connections);
        void        requestType(void);
};

void    HexToDec(const std::string hexValue, size_t & result);