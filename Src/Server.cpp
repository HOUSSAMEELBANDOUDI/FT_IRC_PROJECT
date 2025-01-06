/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:55:38 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/06 19:37:37 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"

Server::Server(){this->_Ser_Socket_Fd = -1;}
Server::Server(Server const &other){*this = other;}
Server &Server::operator=(Server const &other){
    if (this != &other)
    {
        this->_Port = other._Port;
        this->_Password = other._Password;
        this->_Ser_Socket_Fd = other._Ser_Socket_Fd;
        this->_Clients = other._Clients;
        this->_Pfds = other._Pfds;
    }
    return *this;
}
Server::~Server(){};
//---------------//Set METHODES;
void Server::SetFd(int fd){this->_Ser_Socket_Fd = fd;}
void Server::SetPort(int port){this->_Port = port;}
void Server::SetPassword(std::string password){this->_Password = password;}
//---------------//Get METHODES;
int Server::GetFd(){return this->_Ser_Socket_Fd;}
int Server::GetPort(){return this->_Port;}
std::string Server::GetPassword(){return this->_Password;}
Client *Server::GetClient(int fd){
	for (size_t i = 0; i < this->_Clients.size(); i++){
		if (this->_Clients[i].GetFd() == fd)
			return &this->_Clients[i];
	}
	return NULL;
}

//---------------//Signal Methods
bool Server::_Signal = false;
void Server::ft_SignalHandler(int sign)
{
	(void)sign;
	Server::_Signal = true;
	std::cout << std::endl << "Signal has Received!" << std::endl;
}
void Server::ft_RemoveClient(int fd) {
    for (std::vector<Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
        if (it->GetFd() == fd) {
            _Clients.erase(it);
            return;
        }
    }
}
void Server::ft_RemovePfds(int fd) {
    for (std::vector<struct pollfd>::iterator it = _Pfds.begin(); it != _Pfds.end(); ++it) {
        if (it->fd == fd) {
            _Pfds.erase(it);
            return;
        }
    }
}

void	Server::ft_close_Pfds(){
	for(size_t i = 0; i < _Clients.size(); i++){
		std::cout << RED << "Client <" << _Clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(_Clients[i].GetFd());
	}
	if (_Ser_Socket_Fd != -1){	
		std::cout << RED << "Server <" << _Ser_Socket_Fd<< "> Disconnected" << WHI << std::endl;
		close(_Ser_Socket_Fd);
	}
}
void Server::ft_init(int Port, std::string password)
{
    this->_Port = Port;
    this->_Password = password;
    this->ft_set_server_socket();
    std::cout << GRE << "Server <<"<< _Ser_Socket_Fd << ">> Connected" << WHI << std::endl;
    std::cout<<"Waiting to accept a connection..\n";
    while(Server::_Signal == false)//-> run the server until the signal is received
    {
        if((poll(&_Pfds[0], _Pfds.size(), -1) == -1) && Server::_Signal == false) //-> wait for an event
            throw(std::runtime_error("poll() failed"));
        for (size_t i = 0; i < _Pfds.size(); i++)//-> check all file descriptors
        {
            if (_Pfds[i].revents & POLLIN)//-> check if there is data to read
            {
                if (_Pfds[i].fd == _Ser_Socket_Fd)
                    this->ft_accept_new_client_connect();
				else
					this->ft_Receive_New_Data(_Pfds[i].fd);
            }
        }
    }
    ft_close_Pfds();
}
void Server::ft_set_server_socket()
{
    int i = 1;
    _Address.sin_family = AF_INET;//-> set the address family to ipv4
    _Address.sin_addr.s_addr = INADDR_ANY;//-> set the address to any local machine address
    _Address.sin_port = htons(_Port); //-> convert the port to network byte order
    _Ser_Socket_Fd= socket(AF_INET, SOCK_STREAM, 0);//-> create the server socket
    if(_Ser_Socket_Fd == -1)//-> check if the socket is created
        throw(std::runtime_error("Echec to creat socket"));
    if(setsockopt(_Ser_Socket_Fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) == -1)//-> set the socket option (SO_REUSEADDR) to reuse the address
        throw(std::runtime_error("Echec to set option with setsockopt() on socket"));
    if(fcntl(_Ser_Socket_Fd, F_SETFL, O_NONBLOCK) == -1)//-> set the socket option (O_NONBLOCK) for non-blocking socket
        throw(std::runtime_error("Echec to set option with fcntl() on socket"));
    if(bind(_Ser_Socket_Fd, (struct sockaddr *)&_Address, sizeof(_Address)) == -1)//-> bind the socket to the address
        throw(std::runtime_error("Echec to bind() socket"));
    if(listen(_Ser_Socket_Fd, SOMAXCONN) == -1)//-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("Echec to listen()"));
    _New_connect.fd =_Ser_Socket_Fd;//-> add the server socket to the pollfd
    _New_connect.events = POLLIN;//-> set the event to POLLIN for reading data
    _New_connect.revents = 0;//-> set the revents to 0
    _Pfds.push_back(_New_connect); //-> add the server socket to the pollfd
    
}
void Server::ft_accept_new_client_connect()
{
    Client client;//-> create a new client
    memset(&_Client_Add, 0, sizeof(_Client_Add));
    socklen_t len = sizeof(_Client_Add);
    int accfd = accept(_Ser_Socket_Fd, (sockaddr *)&(_Client_Add), &len);//-> accept the new client
    if (accfd == -1)
    {
        std::cout<< "Echec on accept()" << std::endl;
        return ;
    }
    if (fcntl(accfd, F_SETFL, O_NONBLOCK) == -1)//-> set the socket option (O_NONBLOCK) for non-blocking socket
    {
        std::cout << "Echec on fcntl()" << std::endl;
        return ;
    }
    _New_connect.fd = accfd;//-> add the client socket to the pollfd
	_New_connect.events = POLLIN;//-> set the event to POLLIN for reading data
	_New_connect.revents = 0;//-> set the revents to 0
	client.SetFd(accfd);//-> set the client file descriptor
	client.SetIpadd(inet_ntoa((_Client_Add.sin_addr)));//-> convert the ip address to string and set it
	_Clients.push_back(client);//-> add the client to the vector of clients
	_Pfds.push_back(_New_connect);//-> add the client socket to the pollfd
	std::cout << GRE << "Client <" << accfd << "> Connected" << WHI << std::endl;

}

void Server::ft_Receive_New_Data(int fd)
{
    std::vector<std::string> command;
    char buffer[1024];//-> buffer for the received data
    memset(buffer, 0, sizeof(buffer));//-> clear the buffer
    Client *client = GetClient(fd);
    ssize_t bits = recv(fd, buffer, sizeof(buffer) - 1 , 0);//-> receive the data
    if (bits <= 0)
    {
        std::cout << RED << "Client <<" << fd << ">> Disconnected" << WHI << std::endl;
        //remove channels;
        ft_RemoveClient(fd);
        ft_RemovePfds(fd);
        close(fd);
        
    }
}
