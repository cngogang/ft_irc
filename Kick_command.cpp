/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick_command.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:02:19 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/15 17:39:01 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"

int Server::kick_command_channel_checking(std::string channel_name, int user_fd)
{
    if (!is_a_channel(channel_name))
    {
        send_message(user_fd, ERR_BADCHANMASK(channel_name));
        return (0);
    }
    if (this->channels_line.find(channel_name) == this->channels_line.end())
    {
        send_message(user_fd, ERR_NOSUCHCHANNEL(channel_name));	
        return (0);
    }
    if (!this->channels_line[channel_name].Get_operators(user_fd))
    {
        send_message(user_fd, ERR_CHANOPRIVSNEEDED(channel_name));
        return (0);
    }
    return (1);
}

int Server::kick_command_user_checking(std::string channel_name, std::string nick, int user_fd, int & member_kicked_fd)
{
   
    if (this->client_line_by_nick.find(nick) == this->client_line_by_nick.end())
    {
        send_message(user_fd, ERR_USERNOTINCHANNEL(this->client_line[user_fd].get_nick(), nick, channel_name));
        return (0);
    }
    member_kicked_fd = this->client_line_by_nick[nick]->get_fd_socket();
    if (!this->channels_line[channel_name].is_in_the_channel(member_kicked_fd))
    {
        send_message(user_fd, ERR_USERNOTINCHANNEL(this->client_line[user_fd].get_nick(), nick, channel_name));
        return (0);
    }
    return (1);
            
}

static void remove_user_from_channel_and_remove_channel_from_user_channel_list(Channel & channel, Client & client)
{
    std::vector<std::string>::iterator channel_name_pos;
    int client_fd = client.get_fd_socket();
    channel_name_pos =  std::find(client.Channel_list.begin(), client.Channel_list.begin(), channel.Get_name());
    client.Channel_list.erase(channel_name_pos);
    channel.remove_members(client_fd);
}

void Server::command_kick(int fd, Message msg)
{
    std::vector<std::string> list_channel;
    std::vector<std::string> list_user;
    int user_fd = 0;

    if (msg.params.size() < 2 )
        return ;
    list_channel = Server::split_string(msg.params[0], ',');
    list_user = Server::split_string(msg.params[1], ',');

    for (std::vector<std::string>::iterator itc = list_channel.begin(); itc != list_channel.end(); ++itc)
    {
        if (!kick_command_channel_checking(*itc, fd))
            continue ;
        for (std::vector<std::string>::iterator itu = list_user.begin(); itu != list_user.end(); ++itu)
        {   
            if (!kick_command_user_checking(*itc, *itu, fd, user_fd))
                continue ;
            remove_user_from_channel_and_remove_channel_from_user_channel_list(this->channels_line[*itc], *(this->client_line_by_nick[*itu])); 
            send_message(user_fd, RPL_KICK(this->client_line[fd].get_nick(), *itu, *itc,  msg.trailing_params));
            if (!this->channels_line[*itc].get_size())
                this->channels_line.erase(*itc);
            else
                Broadcast_to_the_channel(*itc, RPL_KICK(this->client_line[fd].get_nick(), *itu, *itc,  msg.trailing_params));            
        }
    }
}
