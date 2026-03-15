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

void Server::command_kick(int fd, Message msg)
{
    std::vector<std::string> list_channel;
    std::vector<std::string> list_user;
    std::vector<std::string>::iterator channel_name_pos;
    int user_fd;

    if (msg.params.size() < 2 )
        return ;
    list_channel = Server::split_string(msg.params[0], ',');
    list_user = Server::split_string(msg.params[1], ',');

    for (std::vector<std::string>::iterator itc = list_channel.begin(); itc != list_channel.end(); ++itc)
    {
        if (!is_a_channel(*itc))
            {
                send_message(fd, ERR_BADCHANMASK(*itc));
                continue ;
            }
        if (this->channels_line.find(*itc) == this->channels_line.end())
            {
                send_message(fd, ERR_NOSUCHCHANNEL(*itc));	
                continue ;
            }
        if (!this->channels_line[*itc].Get_operators(fd))
            {
                send_message(fd, ERR_CHANOPRIVSNEEDED(*itc));
                continue ;
            }
        for (std::vector<std::string>::iterator itu = list_user.begin(); itu != list_user.end(); ++itu)
        {   
            
            
            if (this->client_line_by_nick.find(*itu) == this->client_line_by_nick.end())
            {
                send_message(fd, ERR_USERNOTINCHANNEL(this->client_line[fd].get_nick(), *itu, *itc));
                continue ;
            } 
            user_fd = this->client_line_by_nick[*itu]->get_fd_socket();
            if (!this->channels_line[*itc].Get_members(user_fd))
            {
                send_message(fd, ERR_USERNOTINCHANNEL(this->client_line[fd].get_nick(), *itu, *itc));
                continue ;
            }
            channel_name_pos =  std::find(this->client_line_by_nick[*itu]->Channel_list.begin(), this->client_line_by_nick[*itu]->Channel_list.begin(), *itc);
            this->client_line_by_nick[*itu]->Channel_list.erase(channel_name_pos);
            this->channels_line[*itc].remove_members(user_fd);
            if (!this->channels_line[*itc].get_size())
                this->channels_line.erase(*itc);
            else
                warn_the_channel(*itc, RPL_KICK(this->client_line[fd].get_nick(), *itu, *itc,  msg.trailing_params));
            
        }
    }
}
