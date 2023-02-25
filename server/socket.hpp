/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 18:23:52 by mmoumni           #+#    #+#             */
/*   Updated: 2023/02/25 15:57:21 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <poll.h>
#include <vector>
#include <errno.h>

typedef struct addrinfo addrinfo;

class Socket
{
    private:
        std::string Host;
        std::string Port;
        int         socketId;
    public:
        addrinfo    *getinfostruct(std::string hostname, std::string port);
        int         getSockId(addrinfo *hints);
};


#endif
