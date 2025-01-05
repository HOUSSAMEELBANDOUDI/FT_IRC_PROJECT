/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:55:38 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/05 00:08:49 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"

Server::Server(){this->SerSocketFd;}
Server::~Server(){};
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
    if (this != &src)
    {
        this->Port = src.Port;
        this->password = src.password;
        this->SerSocketFd = src.SerSocketFd;
        this->clients = src.clients;
        this->fds = src.fds;
    }
    return *this;
}
//---------------//Set METHODES;
void Server::SetFd(int fd){this->SerSocketFd = fd;}
void Server::SetPort(int port){this->Port = port;}
void Server::SetPassword(std::string password){this->password = password;}
//---------------//Get METHODES;
int Server::GetFd(){return this->SerSocketFd;}
int Server::GetPort(){return this->Port;}
std::string Server::GetPassword(){return this->password;}

//---------------//Signal Methods
bool Server::Signal = false;
void Server::SignalHandler(int sign)
{
	(void)sign;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}
void Server::set_server_socket()
{
    int en = 1;
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = INADDR_ANY;
    add.sin_port = htons(Port);
    SerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(SerSocketFd == -1)
        throw(std::runtime_error("failed to creat socket"));
    if(setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if(fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if(bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1)
        throw(std::runtime_error("faild to bind socket"));
    if(listen(SerSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
    new_cli.fd = SerSocketFd;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    fds.push_back(new_cli);
    
}


