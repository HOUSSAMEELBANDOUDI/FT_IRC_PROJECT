/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:07:05 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/15 22:32:04 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Client.hpp"

Client::Client()
{
    this->fd = -1;
    this->ipadd = "";
    this->_registered = false;
    this->_logedin = false;
    this->_Buffer = "";
    this->_Nickname = "";
    this->_Username = "";
}
Client:: ~Client(){}
Client:: Client(Client const &other)
{
    *this = other;
}
Client &Client::operator=(Client const &other)
{
    if (this != &other)
    {
        this->fd = other.fd;
        this->ipadd = other.ipadd;
        this->_Buffer = other._Buffer;
        this->_registered = other._registered;
        this->_Nickname = other._Nickname;
        this->_Username = other._Username;
        this->_logedin = other._logedin;
    }
    return (*this);
}
//---------------//Set METHODE;
void Client::SetFd(int fd){this->fd = fd;}
void Client::SetIpadd(std::string ipadd){this->ipadd = ipadd;}
void Client::setBuffer(std::string buffer){_Buffer += buffer;}
void Client::SetRegistered(bool value){_registered = value;}
void Client::SetNickname(std::string& Nickname){this->_Nickname = Nickname;}
void Client::SetUsername(std::string& Username){this->_Username = Username;}
void Client::setLogedin(bool value){this->_logedin = value;}
//---------------//Get METHODE
int Client::GetFd(){return this->fd;}
std::string Client::GetIpadd(){return this->ipadd;}
std::string Client::getBuffer(){return this->_Buffer;}
bool Client::getRegistered(){return this->_registered;}
std::string Client::GetNickName(){return this->_Nickname;}
std::string Client::GetUserName(){return this->_Username;}
bool Client::GetLogedIn(){return this->_logedin;}
//--------//other methode
void Client::clearBuffer(){_Buffer.clear();}