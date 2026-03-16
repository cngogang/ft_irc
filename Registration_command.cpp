/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Registration_command.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:37:47 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:54:20 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int Server::is_register(int fd)
{
    
    std::cout << "IR NICK : " << this->client_line[fd].get_nick() << std::endl;
    std::cout << "IR USER : " << this->client_line[fd].get_username() << std::endl;
    std::cout << "IR PASS client : " << this->client_line[fd].get_pass() << " server : " << this->password << " bool == " << (this->password == this->client_line[fd].get_pass())  << std::endl;
    if (this->client_line[fd].get_nick().empty() || this->client_line[fd].get_username().empty())
        return (0);
    if (this->client_line[fd].get_pass() != this->password)
        return (0);
    return (1);

}


void Server::command_nick(int fd, Message msg)
{
    std::string nick;

    
    if (is_register(fd))
    {
        send_message(fd, ERR_ALREADYREGISTRED);
        return ;
    }
    if (msg.params.size() != 1)
    {
        send_message(fd, ERR_NONICKNAMEGIVEN);
        return ;
    }
    nick = Server::trim_white(msg.params[0]);
    if (!Server::is_a_valid_nickname(nick))
    {
        send_message(fd, ERR_ERRONEUSNICKNAME(nick));
        return ;
    }
    if (this->client_line_by_nick.find(nick) != this->client_line_by_nick.end())
    {
        send_message(fd, ERR_NICKNAMEINUSE(nick));
        return ;
    }
    if(!this->client_line[fd].get_nick().empty())
        send_message(fd ,RAW_NICKNAME(this->client_line[fd].get_nick(), nick, this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress()));
    this->client_line[fd].set_nick(nick);
    this->client_line_by_nick[nick] = &(this->client_line[fd]);
    if (is_register(fd))
        welcome_msg(fd);
}


void Server::command_user(int fd, Message msg)
{
    std::string username;
    
    
    if (is_register(fd))
    {
        send_message(fd, ERR_ALREADYREGISTRED);
        return ;
    }
    if (msg.params.size() != 1)
    {
       send_message(fd, ERR_NEEDMOREPARAMS(msg.command));
        return ;
    }
    username = Server::trim_white(msg.params[0]);
    this->client_line[fd].set_username(username);
    if (is_register(fd))
        welcome_msg(fd);
}


// static std::string trim_CRLF(std::string str)
// {
//     std::string::iterator ite;
//     ite = --str.end();
//     while (*ite == '\n' || *ite == '\r' )
//         str.erase(ite--);
//     return (str);
// }
void Server::command_pass(int fd, Message msg)
{
    std::string pass;
    

    
       
    if (is_register(fd))
    {
        send_message(fd, ERR_ALREADYREGISTRED);
        return ;
    }

    if (msg.params.size() != 1)
    {
        send_message(fd, ERR_NEEDMOREPARAMS(msg.command));
        return ;
    }
    pass = Server::trim_white(msg.params[0]);
    if (pass != this->password)
    {
        send_message(fd, ERR_PASSWDMISMATCH);
        return ;
    }
    this->client_line[fd].set_pass(pass);
    if (is_register(fd))
        welcome_msg(fd);
}
