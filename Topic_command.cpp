/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic_command.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 18:01:45 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 19:19:58 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"


void Server::set_topic(int fd, std::string channel_name, std::string topic)
{
    if (this->channels_line.find(channel_name) == this->channels_line.end())
    {
        send_message(fd, ERR_NOSUCHCHANNEL(channel_name));	
        return  ;
    }
    else if (this->channels_line[channel_name].is_in_the_channel(fd))
    {
        send_message(fd, ERR_NOTONCHANNEL(this->client_line[fd].get_nick(), channel_name));
        return ;
    }
    else if (this->channels_line[channel_name].is_topic_restriction() && !this->channels_line[channel_name].Get_operators(fd))
    {
        send_message(fd, ERR_CHANOPRIVSNEEDED(channel_name));
        return ;

    }    
    this->channels_line[channel_name].Set_topic(topic);
    warn_the_channel(channel_name, RAW_TOPIC(this->client_line[fd].get_nick(), this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, this->channels_line[channel_name].Get_topic()));  
}

void Server::display_topic(int fd, std::string channel_name)
{
        if (this->channels_line.find(channel_name) == this->channels_line.end())
        {
            send_message(fd, ERR_NOSUCHCHANNEL(channel_name));	
            return  ;
        }
        else if (!this->channels_line[channel_name].Get_members(fd))
        {
            send_message(fd, ERR_NOTONCHANNEL(this->client_line[fd].get_nick(), channel_name));
            return ;
        }
        else if (this->channels_line[channel_name].Get_topic().empty())
        {
            send_message(fd, RPL_NOTOPIC(this->client_line[fd].get_nick(), channel_name));
            return  ;
        }
        else
            send_message(fd, RPL_TOPIC(this->client_line[fd].get_nick(), channel_name, this->channels_line[channel_name].Get_topic()));

}

void Server::command_topic(int fd, Message msg)
{
    if (msg.trailing_params.empty())
        display_topic(fd, Server::trim_white(msg.params[0]));
    else
        set_topic(fd, Server::trim_white(msg.params[0]), Server::trim_white(msg.trailing_params));
}
