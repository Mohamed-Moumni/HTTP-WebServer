/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/09 17:51:22 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <map>
#include <vector>
#include <unistd.h>

#define BUFFER 64000

class ConnectSocket
{
    private:
        int         ConnectSocketId;
        int         ContentLength;
        int         ResponseLength;
    public:
        bool        ReadAvailble;
        bool        SendAvailble;
        std::string Request;
        ConnectSocket();
        ~ConnectSocket();
        ConnectSocket(int ConnectionId);
        std::string getRededRequest(void);
        std::string getSendedResponse(void);
        std::string getReadRest(void);
        std::string getSendRest(void);
        void        setResponseLength(int length);
        void        read_request(void);
        void        get_content_length(std::string request);
        void        send_response(std::string response);
        void        reset_data(void);
};
