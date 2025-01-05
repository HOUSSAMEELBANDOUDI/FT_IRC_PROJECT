/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:40:58 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/04 23:28:34 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "Client.hpp"

class Client;

class Server
{
  private:
    int Port; //-> server port
	std::string password;//->for authentication
	int SerSocketFd; //-> server socket file descriptor
	static bool Signal; //-> static boolean for signal
	std::vector<Client> clients; //-> vector of clients
	std::vector<struct pollfd> fds; //-> vector of pollfd
	struct sockaddr_in add;//->struct is used to represent an IPv4 address and port number combination
	struct sockaddr_in cliadd;//->for client
	struct pollfd new_cli;//->is a structure used for monitoring file descriptors for I/O events.
  public:
    Server();
    ~Server();
    Server(Server const &src);
    Server &operator=(Server const &src);
    //---------------//Setters
	void SetFd(int server_fdsocket);
	void SetPort(int port);
	void SetPassword(std::string password);
    //---------------//Getters
	int GetFd();
	int GetPort();
	std::string GetPassword();
    //---------------//Server Methods
    void init(int port, std::string pass); //-> server initialization
	void AcceptNewClient(); //-> accept new client
	void set_server_socket(); //->make_socket on work
	void ReceiveNewData(int fd); //-> receive new data from a registered client
    
    static void SignalHandler(int signum); //-> signal handler
 
	void CloseFds(); //-> close file descriptors
	void ClearClients(int fd); //-> clear client
};

#endif