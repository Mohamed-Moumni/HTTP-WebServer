/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/05 16:04:16 by mmoumni          ###   ########.fr       */
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
#include <sstream>
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
        bool            closed;
        bool            sended;
        std::string     IpAdress;
        std::string     Port;
        request         _request;
        response        _response;
        ~ConnectSocket();
        ConnectSocket();
        ConnectSocket(int SocketId, std::string _IpAdress, std::string _port);
        void        readRequest( ConfigFile & _configfile);
        void        readFirst(void);
        void        sendResponse(void);
        void        getContentLength(void);
        void        readChuncked(void);
        void        readUnChuncked(void);
        void        FirstRead(ConfigFile & _configfile);
        void        requestType(void);
        std::string getChuncked(std::string req);
};

void    HexToDec(const std::string hexValue, size_t & result);