/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 21:52:49 by mmoumni           #+#    #+#             */
/*   Updated: 2023/04/11 15:31:25 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server.hpp"

Server::Server()
{
	_client_max_body_size = 1000000;
}

Server::~Server()
{
}