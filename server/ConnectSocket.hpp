/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/26 13:34:34 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <map>
#include <vector>
#include <unistd.h>

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
        std::string     Request;
        ConnectSocket();
        ~ConnectSocket();
        ConnectSocket(int ConnectionId, std::string _IpAdress, std::string _Port);
        std::string getRededRequest(void);
        std::string getSendedResponse(void);
        std::string getReadRest(void);
        std::string getSendRest(void);
        void        setResponseLength(unsigned long length);
        void        read_request(void);
        void        get_content_length(std::string request);
        void        send_response(std::string &response);
        void        reset_data(void);
        void        ConnectionType(std::string _request);
};
