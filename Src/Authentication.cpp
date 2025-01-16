/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 23:43:34 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/15 22:29:27 by hel-band         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Include/Server.hpp"

void Server::ft_client_authentication(int fd, std::string cmd)
{
    Client *client = GetClient(fd);
    // Extract password from the command
    std::string pass = ft_extractPassword(cmd);
    // Check if there is enough data or if it's empty
    if (pass.empty())
    {
        ft_sendErrorResponse(ERR_NOTENOUGHPARAM("*"), fd);
        return ;
    }

    // If client is not registered yet, check the password
    if (!client->getRegistered())
        ft_authenticate_Client(client, pass, fd);
    // If the client is already registered
    else
        ft_sendErrorResponse(ERR_ALREADYREGISTERED(client->GetNickName()), fd);
}

std::string Server::ft_extractPassword(std::string cmd)
{
    // Remove "PASS " prefix from the command and trim any whitespace
    cmd = cmd.substr(4);  // Remove "PASS " prefix
    size_t pos = cmd.find_first_not_of("\t\v ");  // Find first non-whitespace character
    if (pos != std::string::npos) {
        cmd = cmd.substr(pos);  // Remove leading whitespace
        if (cmd[0] == ':')
            cmd.erase(cmd.begin());  // Remove ':' if present
    }
    else 
        return "";  // If no valid characters found, return empty string
    
    return cmd;  // Return the extracted password
}

void Server::ft_sendErrorResponse(std::string response, int fd)
{
    // Send the error response to the client
    if (send(fd, response.c_str(), response.size(), 0) == -1)
        std::cerr << "Error in send() response" << std::endl;
}

void Server::ft_authenticate_Client(Client *client, std::string pass, int fd)
{
    // Check the password and register the client
    if (pass == _Password)
        client->SetRegistered(true);
        //  send success response
    else
        ft_sendErrorResponse(ERR_INCORPASS("*"), fd);
}

void	Server::ft_put_username(std::string& cmd, int fd)
{
	std::vector<std::string> Vcmd = ft_split_command(cmd);

	Client *client = GetClient(fd); 
	if((client && Vcmd.size() < 5))
		{ft_sendErrorResponse(ERR_NOTENOUGHPARAM(client->GetNickName()), fd); return; }
	if(!client  || !client->getRegistered())
		ft_sendErrorResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (client && !client->GetUserName().empty())
		{ft_sendErrorResponse(ERR_ALREADYREGISTERED(client->GetNickName()), fd); return;}
	else
		client->SetUsername(Vcmd[1]);
	if(client && client->getRegistered() && !client->GetUserName().empty() && !client->GetNickName().empty()
        && client->GetNickName() != "*"  && !client->GetLogedIn())
	{
		client->setLogedin(true);
		ft_sendErrorResponse(RPL_CONNECTED(client->GetNickName()), fd);
	}
}

bool Server::ft_isValidNickname(const std::string& nickname) const
{
    if (nickname.empty() || nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':')
        return false;

    for (size_t i = 0; i < nickname.size(); i++)
    {
        if (!std::isalnum(nickname[i]) && nickname[i] != '_')
            return false;
    }
    return true;
}

bool Server::ft_isNicknameInUse(std::string& nickname) {
    for (std::vector<Client>::iterator it  = _Clients.begin(); it != _Clients.end(); ++it) {
        if (it->GetNickName() == nickname)
            return true;
    }
    return false;
}

void Server::ft_setNickname(std::string& cmd, int fd)
{
    std::string nickname = ft_extractNickname(cmd);
    if (nickname.empty())
    {
       ft_sendErrorResponse(ERR_NOTENOUGHPARAM("*"), fd);
        return;
    }

    Client* cli = GetClient(fd);
    if (cli == NULL) // Use NULL instead of nullptr
        return;

    if (ft_isNicknameInUse(nickname) && cli->GetNickName() != nickname)
    {
        ft_handleNicknameInUse(cli, nickname, fd);
        return;
    }

    if (!ft_isValidNickname(nickname))
    {
       ft_sendErrorResponse(ERR_ERRONEUSNICK(nickname), fd);
        return;
    }

    ft_processNicknameChange(cli, nickname, fd);
}

std::string Server::ft_extractNickname(const std::string& cmd) const
{
    std::string nickname = cmd.substr(4); // Remove "NICK"
    size_t pos = nickname.find_first_not_of("\t\v ");
    if (pos != std::string::npos)
    {
        nickname = nickname.substr(pos);
        if (!nickname.empty() && nickname[0] == ':')
            nickname.erase(nickname.begin());
    }
    return nickname;
}

void Server::ft_handleNicknameInUse(Client* cli,  std::string& nickname, int fd)
{
    std::string temp = "*";
    if (cli->GetNickName().empty())
        cli->SetNickname(temp);

   ft_sendErrorResponse(ERR_NICKINUSE(nickname), fd);
}

void Server::ft_processNicknameChange(Client* cli,  std::string& nickname, int fd)
{
    std::string oldNickname = cli->GetNickName();

    // Update nickname
    cli->SetNickname(nickname);
    //ft_updateChannelsNickname(oldNickname, nickname);//->update nikname in chaneel

    if (!oldNickname.empty() && oldNickname != nickname)
    {
        ft_handleNicknameUpdate(cli, oldNickname, nickname, fd);
    }

    if (cli->getRegistered() && !cli->GetLogedIn() && !cli->GetUserName().empty())
    {
        cli->setLogedin(true);
       ft_sendErrorResponse(RPL_CONNECTED(nickname), fd);
    }
}

// void Server::ft_updateChannelsNickname(const std::string& oldNickname, const std::string& newNickname)
// {
//     for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
//     {
//         Client* cl = it->GetClientInChannel(oldNickname);
//         if (cl != NULL)
//             cl->SetNickname(newNickname);
//     }
// }

void Server::ft_handleNicknameUpdate(Client* cli, const std::string& oldNickname, const std::string& newNickname, int fd)
{
    if (oldNickname == "*" && !cli->GetUserName().empty())
    {
        cli->setLogedin(true);
       ft_sendErrorResponse(RPL_CONNECTED(newNickname), fd);
       ft_sendErrorResponse(RPL_NICKCHANGE(oldNickname, newNickname), fd);
    }
    else
    {
       ft_sendErrorResponse(RPL_NICKCHANGE(oldNickname, newNickname), fd);
    }
}

