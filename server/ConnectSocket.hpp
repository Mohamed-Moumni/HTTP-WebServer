/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 13:30:26 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/06 14:19:51 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTSOCKET_HPP
#define CONNECTSOCKET_HPP

#include <iostream>

class ConnectSocket
{
    private:
        int         ConnctSocketId;
        std::string readedRequest;
        std::string readedRest;
        std::string sendedRespons;
        std::string send_rest;
        bool        readAvailble;
        bool        sendAvailble;
    public:
        ConnectSocket();
        ~ConnectSocket();
        
        std::string getRededRequest(void);
        std::string getSendedResponse(void);
        std::string getReadRest(void);
        std::string getSendRest(void);
        void        read_request(void);
        
};

#endif