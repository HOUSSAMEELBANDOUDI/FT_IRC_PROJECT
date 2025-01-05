/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:07:05 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/04 20:54:40 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Client.hpp"

Client::Client()
{
    this->fd = -1;
    this->ipadd = "";
}
Client:: ~Client(){}
Client:: Client(Client const &src)
{
    *this = src;
}
Client &Client::operator=(Client const &src)
{
    if (this != &src)
    {
        this->fd = src.fd;
        this->ipadd = src.ipadd;
    }
    return (*this);
}
//---------------//Set METHODE;
void Client::SetFd(int fd){this->fd = fd;}
void Client::SetIpadd(std::string ipadd){this->ipadd = ipadd;}
//---------------//Get METHODE
int Client::GetFd(){return this->fd;}
std::string Client::GetIpadd(){return this->ipadd;}