/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join_command.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:11:25 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:48:22 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"

void Server::create_a_new_channel(std::map<std::string, Channel> & channel_line, Client & client, std::string channel_name_trim)
{

        std::cout << "creation channel "<< channel_name_trim << std::endl; 
        channel_line[channel_name_trim] =  Channel(client , client.get_fd_socket(), channel_name_trim);
        channel_line[channel_name_trim].add_operators(client , client.get_fd_socket());
        channel_line[channel_name_trim].Set_name(channel_name_trim);
        client.Channel_list.push_back(channel_name_trim);
}


void Server::warn_the_channel(std::string channel_name, std::string msg)
{
    std::map<int, Client *> operators_list = this->channels_line[channel_name].Get_operators();
    std::map<int, Client *> members_list = this->channels_line[channel_name].Get_members();

    for (std::map<int, Client *>::iterator it = operators_list.begin(); it != operators_list.end(); ++it)
    {
        send_message((*it).first, msg);
    }
    for (std::map<int, Client *>::iterator it = members_list.begin(); it != members_list.end(); ++it)
    {
        send_message((*it).first, msg);
    }

}


void Server::join_channel(int client_fd, std::string channel_name, std::string key)
{
    std::string channel_name_trim = Server::trim_white(channel_name);
    if (!is_a_valid_channel_name(channel_name_trim))
      {
        send_message(client_fd, ERR_BADCHANMASK(channel_name));
        return ;        
      }
    if (this->channels_line.find(channel_name_trim) == this->channels_line.end())
       create_a_new_channel(this->channels_line, this->client_line[client_fd], channel_name_trim);
    else
    {
        
        if(this->channels_line[channel_name_trim].is_locked() && key != this->channels_line[channel_name_trim].Get_key())
            send_message(client_fd, ERR_BADCHANNELKEY(channel_name));
        else if (this->channels_line[channel_name_trim].is_limited() && this->channels_line[channel_name_trim].is_limited() >= this->channels_line[channel_name_trim].get_size())
            send_message(client_fd, ERR_CHANNELISFULL(channel_name));
        else if(this->channels_line[channel_name_trim].add_members(this->client_line[client_fd], client_fd) == -1)
            send_message(client_fd, ERR_INVITEONLYCHAN(channel_name));
        else 
        {
            this->client_line[client_fd].Channel_list.push_back(channel_name_trim);
            warn_the_channel(channel_name_trim, RAW_JOIN(this->client_line[client_fd].get_nick(), this->client_line[client_fd].get_username(), this->client_line[client_fd].get_IP_adress(), channel_name));
            join_names_reply(client_fd,  channel_name_trim);
        }

    }     
}



void Server::command_join(int fd, Message msg)
{
    std::vector<std::string> list_channel = Server::split_string(msg.params[0], ',');
    std::vector<std::string> keys;
    std::vector<std::string>::iterator key;

    if (msg.params.size() > 2)
    {
        keys = Server::split_string(msg.params[1], ',');
        key = keys.begin();
        
    }        

    for (std::vector<std::string>::iterator it = list_channel.begin(); it != list_channel.end(); ++it)
    {
        if (!keys.empty())
        {
            if (key == keys.end())
                --key;
            join_channel(fd, *it, Server::trim_white(*key));
            ++key;
        }
        else
            join_channel(fd, *it, std::string(""));


    }   
}


void Server::join_names_reply(int fd, std::string channel_name)
{
    std::string names;
    Server::add_channel_client_to_string(this->channels_line[channel_name], names);
    send_message(fd, RPL_NAMREPLY(this->client_line[fd].get_nick(), channel_name, names));
    send_message(fd, RPL_ENDOFNAMES(this->client_line[fd].get_nick(), channel_name));
}



void Server::command_names(int fd, Message msg)
{
    std::vector<std::string> list_channel;
    std::string command_response = ":ft_irc 353 " + this->client_line[fd].get_nick() + ":";
    if (!msg.params.size())
    {
        list_channel = this->client_line[fd].Channel_list;
        for (std::map<std::string, Channel>::iterator it = this->channels_line.begin(); it != this->channels_line.end(); ++it)
        {
            if (!(*it).second.is_private() && std::find(list_channel.begin(), list_channel.end(), (*it).first) == list_channel.end())
                list_channel.push_back((*it).first);
        }
    }
    else
        list_channel = Server::split_string(msg.params[0], ',');
    for (std::vector<std::string>::iterator it = list_channel.begin(); it != list_channel.end(); it++)
    {

        if (this->channels_line[Server::trim_white(*it)].is_private()) 
            command_response += " * " + this->channels_line[Server::trim_white(*it)].Get_name() + " :";
        else
            command_response += " = " + this->channels_line[Server::trim_white(*it)].Get_name() + " :";
        Server::add_channel_client_to_string(this->channels_line[Server::trim_white(*it)], command_response);
    }
    split_msg_and_send_it(fd,  command_response);
}

