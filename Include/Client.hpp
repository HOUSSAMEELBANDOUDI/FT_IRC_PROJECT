/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:54:33 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/08 20:33:00 by hel-band         ###   ########.fr       */
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
    std::string _Buffer;
    bool _registered;
    std::string _Nickname;
    std::string _Username;
    
  public:
    Client();
    ~Client();
    Client(Client const &src);
    Client &operator=(Client const &src);
    //---------------//Setters
	void SetFd(int fd);
  void SetIpadd(std::string ipadd);
  void setBuffer(std::string Buffer);
  void SetRegistered(bool value);
  void SetNickname(std::string& Nickname);
  void SetUsername(std::string& Username);
    //---------------//Getters
	int GetFd();
  std::string GetIpadd();
  std::string getBuffer();
  bool getRegistered();
  std::string GetNickName();
  std::string GetUserName();
  //---------//other methods
  void clearBuffer();
  
};

#endif