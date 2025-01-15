/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:40:58 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/14 21:23:10 by hel-band         ###   ########.fr       */
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
#include "Message.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Client;

class Server
{
  private:
    int _Port; //-> server port
	std::string _Password;//->for authentication
	int _Ser_Socket_Fd; //-> server socket file descriptor
	static bool _Signal; //-> static boolean for signal
	std::vector<Client> _Clients; //-> vector of clients
	std::vector<struct pollfd> _Pfds; //-> vector of pollfd
	struct sockaddr_in _Address;//->struct is used to represent an IPv4 address and port number combination
	struct sockaddr_in _Client_Add;//->for client
	struct pollfd _New_connect;//->is a structure used for monitoring file descriptors for I/O events.
  public:
    Server();
    ~Server();
    Server(Server const &other);
    Server &operator=(Server const &other);
    //---------------//Setters
	void SetFd(int Ser_Socket_Fd);
	void SetPort(int Port);
	void SetPassword(std::string Password);
    //---------------//Getters
	int GetFd();
	int GetPort();
	std::string GetPassword();
	Client *GetClient(int fd);
    //---------------//Server Methods
    void ft_init(int Port, std::string Password); //-> server initialization
	void ft_accept_new_client_connect(); //-> accept new client
	void ft_register_client(int accfd);
	void ft_set_server_socket(); //->make_socket on work
	void ft_Receive_New_Data(int fd); //-> receive new data from a registered client
	void ft_handleClientDisconnection(int fd);
	void ft_processClientBuffer(int fd, const char *buff);
	void ft_executeCommands( std::vector<std::string> &commands, int fd);
	//----------//Parsing Methodes
	std::vector<std::string> ft_split_buffer(std::string buff);
	std::vector<std::string> ft_split_command(std::string& cmd);
	bool ft_isregistered(int fd);
	void ft_parse_exec_cmd(std::string &command, int fd);
	bool ft_isValid_Port(std::string port);
    //-----//REMOVE 
	void ft_RemoveClient(int fd);
	void ft_RemovePfds(int fd);
    static void ft_SignalHandler(int signum); //-> signal handler
 
	void ft_close_Pfds(); //-> close file descriptors
	void ft_Clear_Clients(int fd); //-> clear client
	
	//--------//AUTHENTICATION
	void ft_client_authentication(int fd, std::string commmande);
	std::string ft_extractPassword(std::string cmd);
	void ft_authenticate_Client(Client *client, std::string pass, int fd);
	void ft_sendErrorResponse(std::string error, int fd);
	void ft_put_username(std::string& cmd, int fd);
};

#endif