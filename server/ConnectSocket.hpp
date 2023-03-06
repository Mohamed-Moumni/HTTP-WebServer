/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/06 15:29:14 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTSOCKET_HPP
#define CONNECTSOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <map>

#define BUFFER 1024

class ConnectSocket
{
    private:
        int         ConnectSocketId;
        int         ContentLength;
        int         char_sent;
        int         response_Length;
        bool        ContentLengthAvailble;
        bool        readAvailble;
        bool        sendAvailble;
        std::string readedRequest;
        std::string sendedRespons;
    public:
        ConnectSocket();
        ~ConnectSocket();
        ConnectSocket(int ConnectionId);
        std::string getRededRequest(void);
        std::string getSendedResponse(void);
        std::string getReadRest(void);
        std::string getSendRest(void);
        void        read_request(void);
        void        get_content_length(std::string request);
        void        send_response(std::string response);
};


#endif