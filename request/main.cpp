/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkarim <mkarim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 13:09:44 by mkarim            #+#    #+#             */
/*   Updated: 2023/02/19 17:38:29 by mkarim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

int main(int argc, char** argv)
{
	std::ifstream		request(argv[1]);
	std::string			data;
	std::string			data_req;

	while (getline(request, data))
	{
		data_req += data;
		data_req += '\n';
	}
	start_request(data_req);
	return 0;
}