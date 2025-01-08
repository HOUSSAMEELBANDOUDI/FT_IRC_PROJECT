/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-band <hel-band@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 23:43:34 by hel-band          #+#    #+#             */
/*   Updated: 2025/01/08 16:05:25 by hel-band         ###   ########.fr       */
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