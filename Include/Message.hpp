/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:49:23 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/08 18:08:57 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#define ENL "\r\n"

#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not Enough Parameters." + ENL)
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You are Already Registered!" + ENL )
#define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Sorry Password Incorrect !" + ENL )
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You haven't Registered!" + ENL)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Command Not Find" + ENL)
#endif
