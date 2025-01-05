/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:54:33 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/05 14:33:17 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
  private:
    int fd; //-> client file descriptor
	  std::string ipadd; //-> client ip address
    
  public:
    Client();
    ~Client();
    Client(Client const &src);
    Client &operator=(Client const &src);
    //---------------//Setters
	void SetFd(int fd);
  void SetIpadd(std::string ipadd);
    //---------------//Getters
	int GetFd();
  std::string GetIpadd();
};

#endif