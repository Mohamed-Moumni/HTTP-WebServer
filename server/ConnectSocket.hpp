/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/27 08:25:03 by mmoumni          ###   ########.fr       */
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
    private:
        int             ConnectSocketId;
        int             ContentLength;
    public:
        unsigned long   ResponseLength;
        bool            ReadAvailble;
        bool            SendAvailble;
        bool            ConnectionClosed;
        std::string     IpAdress;
        std::string     Port;
        request         _request;
        response        _response;
        ConnectSocket();
        ~ConnectSocket();
};
