/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part_command.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:20:38 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:52:30 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::part_channel(int client_fd, std::string channel_name)
{
    std::string channel_name_trim = Server::trim_white(channel_name);
    std::vector<std::string>::iterator channel_name_pos =  std::find(this->client_line[client_fd].Channel_list.begin(), this->client_line[client_fd].Channel_list.begin(), channel_name_trim); 
    if (!is_a_channel(channel_name_trim))
        return ;
    std::cout << "join channel 2" << std::endl;
    if (this->channels_line.find(channel_name_trim) == this->channels_line.end()) 
    {
        send_message(client_fd, ERR_NOSUCHCHANNEL(channel_name_trim));
        return ;
    }
    else if(channel_name_pos == this->client_line[client_fd].Channel_list.end())
        {
            send_message(client_fd, ERR_NTONCHANNEL(this->client_line[client_fd].get_nick(), channel_name_trim));
            return ;
        }
    else
        {
            std::cout << "add member channel "<< channel_name_trim  << std::endl;
            this->channels_line[channel_name_trim].remove_members(client_fd);
            this->client_line[client_fd].Channel_list.erase(channel_name_pos);
            if (!this->channels_line[channel_name_trim].get_size())
                this->channels_line.erase(channel_name_trim);
        }
}



void Server::command_part(int fd, Message msg)
{
    std::vector<std::string> list_channel = Server::split_string(msg.params[0], ',');
    for (std::vector<std::string>::iterator it = list_channel.begin(); it != list_channel.end(); ++it)
    {
        part_channel(fd, *it);
    }   
}
