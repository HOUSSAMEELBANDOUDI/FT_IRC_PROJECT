/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:27:47 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/19 10:31:47 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Include/Server.hpp"

int main(int ac, char **av)
{
	Server server;
	if (ac != 3)
	{
		std::cout << " For Use : " << av[0] << " Enter: <port number> <password>" << std::endl; 
		return 1;
	}
	std::cout << "**** SERVER ****" << std::endl;
	try
	{
		signal(SIGINT, server.ft_SignalHandler);
		signal(SIGQUIT, server.ft_SignalHandler);
		if(!server.ft_isValid_Port(av[1]) || !*av[2] || std::strlen(av[2]) > 20)
			{std::cout << "invalid Port number || Password!" << std::endl; return 1;}
		server.ft_init(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		server.ft_close_Pfds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}