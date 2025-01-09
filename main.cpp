/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:27:47 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/09 20:00:53 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Include/Server.hpp"

bool isPortValid(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

int main(int ac, char **av)
{
	Server ser;
	if (ac != 3)
		{std::cout << "Usage: " << av[0] << " <port number> <password>" << std::endl; return 1;}
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::ft_SignalHandler);
		signal(SIGQUIT, Server::ft_SignalHandler);
		signal(SIGPIPE, SIG_IGN); // or MSG_NOSIGNAL flag in send() to ignore SIGPIPE on linux systems
		if(!isPortValid(av[1]) || !*av[2] || std::strlen(av[2]) > 20)
			{std::cout << "invalid Port number / Password!" << std::endl; return 1;}
		ser.ft_init(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		ser.ft_close_Pfds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}