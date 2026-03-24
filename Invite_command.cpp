/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite_command.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 17:21:28 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/15 18:39:22 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"


int Server::invite_arg_checking(int fd, std::string member_name, std::string channel_name)
{
    if (member_name.empty() || channel_name.empty())
    {
        send_message(fd, ERR_NEEDMOREPARAMS(std::string("INVITE")));
        return (0);
    }
    if (!is_a_channel(channel_name))
    {
        send_message(fd, ERR_BADCHANMASK(channel_name));
        return (0);
    }
    if (this->channels_line.find(channel_name) == this->channels_line.end())
    {
        send_message(fd, ERR_NOSUCHCHANNEL(channel_name));	
        return (0);
    }
    if (!this->channels_line[channel_name].Get_operators(fd))
    {
        send_message(fd, ERR_CHANOPRIVSNEEDED(channel_name));
        return (0);
    }
    if (this->client_line_by_nick.find(member_name) == this->client_line_by_nick.end())
    {
        
        send_message(fd, ERR_NOSUCHNICK(member_name));
        return (0);
    }
    return (1);
    
}

void Server::invite_member(int fd, std::string member_name, std::string channel_name)
{
    int member_to_invite_fd = this->client_line_by_nick[member_name]->get_fd_socket();
    std::string inviter_name = this->client_line[fd].get_nick();
    

    this->channels_line[channel_name].add_to_invitation_list(member_to_invite_fd);
    send_message(fd, RPL_INVITING(inviter_name, member_name, channel_name));
    send_message(member_to_invite_fd, INVITE_MESSAGE(inviter_name, member_name, channel_name));

}

void Server::command_invite(int fd, Message msg)
{
    std::string member_name = trim_white(msg.params[0]);
    std::string channel_name = trim_white(msg.params[1]);
    
    if (invite_arg_checking(fd, member_name, channel_name))
        invite_member(fd, member_name, channel_name);
}