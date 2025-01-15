/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:55:38 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/14 20:59:59 by hel-band         ###   ########.fr       */
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
bool Server::ft_isValid_Port(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}
void Server::ft_RemoveClient(int fd) {
    for (std::vector<Client>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
        if (it->GetFd() == fd) {
            _Clients.erase(it);
            return ;
        }
    }
}
void Server::ft_RemovePfds(int fd) {
    for (std::vector<struct pollfd>::iterator it = _Pfds.begin(); it != _Pfds.end(); ++it) {
        if (it->fd == fd) {
            _Pfds.erase(it);
            return ;
        }
    }
}

void	Server::ft_close_Pfds()
{
	for(size_t i = 0; i < _Clients.size(); i++){
		std::cout << RED << "Client <<" << _Clients[i].GetFd() << ">> Disconnected" << WHI << std::endl;
		close(_Clients[i].GetFd());
	}
	if (_Ser_Socket_Fd != -1){	
		std::cout << RED << "Server <<" << _Ser_Socket_Fd<< ">> Disconnected" << WHI << std::endl;
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
            throw(std::runtime_error("Echec on poll()"));
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
    int opt  = 1;
    _Address.sin_family = AF_INET;//-> set the address family to ipv4
    _Address.sin_addr.s_addr = INADDR_ANY;//-> set the address to any local machine address
    _Address.sin_port = htons(_Port); //-> convert the port to network byte order
    _Ser_Socket_Fd= socket(AF_INET, SOCK_STREAM, 0);//-> create the server socket
    if(_Ser_Socket_Fd == -1)//-> check if the socket is created
        throw(std::runtime_error("Echec to creat socket"));
    if(setsockopt(_Ser_Socket_Fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)//-> set the socket option (SO_REUSEADDR) to reuse the address
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
    memset(&_Client_Add, 0, sizeof(_Client_Add));//->Clear the _Client_Add structure
    socklen_t len = sizeof(_Client_Add);
    int accfd = accept(_Ser_Socket_Fd, (sockaddr *)&_Client_Add, &len);//-> Accept the new client connection
    if (accfd == -1)
    {
        std::cerr << "Error: Failed to accept new client connection." << std::endl;
        return;
    }
    // Set the socket to non-blocking mode
    if (fcntl(accfd, F_SETFL, O_NONBLOCK) == -1)//-> Set the socket to non-blocking mode
    {
        std::cerr << "Error: Failed to set socket to non-blocking mode." << std::endl;
        return;
    }
    ft_register_client(accfd); // Register the new client
}
void Server::ft_register_client(int accfd)
{
    //-> Create and initialize the new client
    Client newClient;
    
    newClient.SetFd(accfd);
    newClient.SetIpadd(inet_ntoa(_Client_Add.sin_addr));
    // Add the new client to the clients vector
    _Clients.push_back(newClient);
    //-> Configure the pollfd structure
    _New_connect.fd = accfd;
    _New_connect.events = POLLIN;
    _New_connect.revents = 0;
    //-> Add the pollfd to the pollfd vector
    _Pfds.push_back(_New_connect);
    std::cout << GRE << "Client <" << accfd << "> Connected from "
              << inet_ntoa(_Client_Add.sin_addr) << WHI << std::endl;
}
void Server::ft_Receive_New_Data(int fd)
{
    char buff[1024];
    
    memset(buff, 0, sizeof(buff));
    ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);//-> receive data from client
    if (bytes <= 0) {
        ft_handleClientDisconnection(fd);
    } else {
        ft_processClientBuffer(fd, buff);
    }
}

void Server::ft_handleClientDisconnection(int fd)
{
    std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
   //->add here function for RmoveChannels(fd);
    ft_RemoveClient(fd);
    ft_RemovePfds(fd);
    close(fd);
}

void Server::ft_processClientBuffer(int fd, const char *buff)
{
    Client *client = GetClient(fd);
    if (!client) return;
    client->setBuffer(buff);
    if (client->getBuffer().find_first_of("\r\n") == std::string::npos) {
        return;  // Incomplete command, wait for more data
    }
    std::vector<std::string> commands = ft_split_buffer(client->getBuffer());
    ft_executeCommands(commands, fd);
    // Clear the client's buffer if the client still exists  
    if (GetClient(fd)) {
        client->clearBuffer();
    }
}

void Server::ft_executeCommands(std::vector<std::string> &commands, int fd)
{
    for (size_t i = 0; i < commands.size(); ++i) {
        ft_parse_exec_cmd(commands[i], fd);
    }
}

std::vector<std::string> Server::ft_split_buffer(std::string buff)
{
    std::vector<std::string> Vbuff;
    std::string data;
    std::istringstream iss(buff);
    while(std::getline(iss, data))
    {
        size_t pos = data.find_first_of("\r\n");
        if (pos != std::string::npos)
            data = data.substr(0, pos);
        Vbuff.push_back(data);
    }
    return (Vbuff);
}

std::vector<std::string> Server::ft_split_command(std::string& command)
{
	std::vector<std::string> Vcmd;
	std::istringstream iss(command);
	std::string data;
	while(iss >> data)
	{
		Vcmd.push_back(data);
		data.clear();
	}
	return Vcmd;
}
bool Server::ft_isregistered(int fd)
{
	if (!GetClient(fd) || GetClient(fd)->GetNickName().empty() || GetClient(fd)->GetUserName().empty()
        || GetClient(fd)->GetNickName() == "*"  || !GetClient(fd)->getRegistered())
		return false;
	return true;
}

void Server::ft_parse_exec_cmd(std::string &cmd, int fd)
{
    if (cmd.empty())
        return ;
    // Trim leading whitespace
    size_t found = cmd.find_first_not_of(" \t\v");
    if (found != std::string::npos)
        cmd = cmd.substr(found);
    // Split the command into tokens
    std::vector<std::string> splited_cmd = ft_split_command(cmd);
    if (splited_cmd.empty())
        return;
    // Normalize the command name (convert to uppercase for comparison)
    std::string command = splited_cmd[0];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    // Handle authentication commands
    if (command == "BONG")
        return;
    if (command == "PASS")
    {
        ft_client_authentication(fd, cmd);
        return;
    }
    if (command == "NICK")
    {
        //set_nickname(cmd, fd);
        return;
    }
    if (command == "USER")
    {
        ft_put_username(cmd, fd);
        return;
    }
    if (command == "QUIT")
    {
        //QUIT(cmd, fd);
        return;
    }

    // Check registration status
    if (!ft_isregistered(fd))
    {
        ft_sendErrorResponse(ERR_NOTREGISTERED(std::string("*")), fd);
        return;
    }

    // // Handle registered user commands
    // if (command == "KICK")
    //     //KICK(cmd, fd);
    // else if (command == "JOIN")
    //     //JOIN(cmd, fd);
    // else if (command == "TOPIC")
    //     //Topic(cmd, fd);
    // else if (command == "MODE")
    //    // mode_command(cmd, fd);
    // else if (command == "PART")
    //    // PART(cmd, fd);
    // else if (command == "PRIVMSG")
    //    // PRIVMSG(cmd, fd);
    // else if (command == "INVITE")
    //     //Invite(cmd, fd);
    // else
    //     ft_sendErrorResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(), command), fd);
}

