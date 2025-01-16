/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:49:23 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/15 22:09:24 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

#define ENL "\r\n"

#define ERR_NOTENOUGHPARAM(nickname) (std::string(": 461 ") + nickname + " :Not Enough Parameters." + ENL)
#define ERR_ALREADYREGISTERED(nickname) (std::string(": 462 ") + nickname + " :You are Already Registered!" + ENL)
#define ERR_INCORPASS(nickname) (std::string(": 464 ") + nickname + " :Sorry Password Incorrect !" + ENL)
#define ERR_NOTREGISTERED(nickname) (std::string(": 451 ") + nickname + " :You haven't Registered!" + ENL)
#define ERR_CMDNOTFOUND(nickname, command) (std::string(": 421 ") + nickname + " " + command + " :Command Not Found" + ENL)
#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + ENL)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + ENL)
#define ERR_ERRONEUSNICK(nickname) (": 432 " + nickname + " :Erroneus nickname" + ENL)
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + ENL)

#endif
