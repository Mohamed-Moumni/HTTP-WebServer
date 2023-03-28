/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/27 11:43:35 by mmoumni          ###   ########.fr       */
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
        int             ConnectSocketId;
        bool            ReadAvailble;
        bool            SendAvailble;
        std::string     IpAdress;
        std::string     Port;
        request         _request;
        response        _response;
        ConnectSocket(int SocketId, std::string _IpAdress, std::string _port);
        ConnectSocket();
        ~ConnectSocket();
};
