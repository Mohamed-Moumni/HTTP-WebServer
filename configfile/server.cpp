/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumni <mmoumni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 21:52:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/03/03 21:52:50 by mmoumni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server.hpp"

Server::Server()
{
	// std::cout << "Server Default Constructor" << std::endl;
	_client_max_body_size = 1;
}

Server::~Server()
{
	// std::cout << "Server Destrucotr" << std::endl;
}