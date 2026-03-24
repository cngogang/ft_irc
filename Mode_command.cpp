/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode_command.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 18:17:23 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/15 18:17:24 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"


static int is_known_mode(char c)
{
    if (c != 'i'  && c!= 't' && c != 'k' && c != 'o' && c != 'l')
        return (0);
    return (1);
    
}
static int mode_atoi(std::string str)
{
    int return_val = 0;
    
    if (str.size() > 9)
        return (-1);
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (*it < '0' || *it > '9')
            return (-1) ;
        return_val = return_val * 10 + (*it - 48);
    }
    return (return_val);
}

static int is_valid_mode_key(std::string key)
{
    if (*(key.begin()) != '+' && *(key.begin()) != '-')
        return (0);
    else if (key.size() != 2)
        return (0);
    else if (!is_known_mode(*(key.begin() + 1)))
        return (0);
    return (1);

}

void Server::command_mode_option_invite_only(int fd, Message msg)
{
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key = Server::trim_white(msg.params[1]);
    std::string nickname = this->client_line[fd].get_nick();
    std::string username = this->client_line[fd].get_username();
    std::string IP = this->client_line[fd].get_IP_adress();

    if (key[0] == '-')
        this->channels_line[channel_name].Put_off_invite_only();
    else
        this->channels_line[channel_name].Put_on_invite_only();
    send_message(fd, RAW_MODE(nickname, username, IP, channel_name, key, std::string("")));    
}

void Server::command_mode_option_topic_privilege(int fd, Message msg)
{
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key = Server::trim_white(msg.params[1]);
    std::string nickname = this->client_line[fd].get_nick();
    std::string username = this->client_line[fd].get_username();
    std::string IP = this->client_line[fd].get_IP_adress();

    if (key[0] == '-')
        this->channels_line[channel_name].Put_off_topic_privilege();
    else
        this->channels_line[channel_name].Put_on_topic_privilege();
    send_message(fd, RAW_MODE(nickname, username, IP, channel_name, key, std::string("")));
}

void Server::command_mode_option_key(int fd, Message msg)
{
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key =  Server::trim_white(msg.params[1]);
    std::string new_pass = msg.params.size() > 2 ? Server::trim_white(msg.params[2]) : "";
    std::string nickname = this->client_line[fd].get_nick();
    std::string username = this->client_line[fd].get_username();
    std::string IP = this->client_line[fd].get_IP_adress();
    
    if (key[0] == '-')
    {
        this->channels_line[channel_name].Set_key(std::string(""));
         Broadcast_to_the_channel(channel_name, RAW_MODE(this->client_line[fd].get_nick(), 
        this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, key, new_pass));
        return ;
    }
    else if (new_pass.empty() || has_white_space(new_pass))
    {
        send_message(fd, std::string("461 MODE :Not enough parameters"));
        return ;
    }
    this->channels_line[channel_name].Set_key(new_pass);
    Broadcast_to_the_channel(channel_name, RAW_MODE(this->client_line[fd].get_nick(), 
    this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, key, new_pass));
}

void Server::command_mode_option_operator(int fd, Message msg)
{
    std::string member_nick = trim_white(msg.params[2]); 
    int user_id;
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key = Server::trim_white(msg.params[1]);
    std::string nickname = this->client_line[fd].get_nick();
    std::string username = this->client_line[fd].get_username();
    std::string IP = this->client_line[fd].get_IP_adress();
    
    if(member_nick.empty())
    {
        send_message(fd, std::string("461 MODE :Not enough parameters"));
        return ;
    }
    else if (this->client_line_by_nick.find(member_nick) == this->client_line_by_nick.end())
    {
        send_message(fd, ERR_NOSUCHNICK(member_nick));
        return ;
    }
    user_id = this->client_line_by_nick[member_nick]->get_fd_socket();
    if (!this->channels_line[channel_name].is_in_the_channel(user_id))
    {
         send_message(fd, ERR_USERNOTINCHANNEL(nickname, member_nick, channel_name));
         return ;
    }
    if (key[0] == '-')
    {
        this->channels_line[channel_name].remove_members(user_id);
        this->channels_line[channel_name].add_members(*(this->client_line_by_nick[member_nick]), user_id);
        Broadcast_to_the_channel(channel_name, RAW_MODE_REMOP(nickname, username, IP,  channel_name, member_nick));
        return ;
    }
    
    this->channels_line[channel_name].remove_members(user_id);
    this->channels_line[channel_name].add_operators(*(this->client_line_by_nick[member_nick]), user_id);
    Broadcast_to_the_channel(channel_name, RAW_MODE_ADDOP(nickname, username, IP,  channel_name, member_nick));
}

void Server::command_mode_option_user_limit(int fd, Message msg)
{
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key = Server::trim_white(msg.params[1]);
    std::string limit = (msg.params.size() > 2) ? Server::trim_white(msg.params[2]) : "";
    int limit_int = mode_atoi(limit);

    if (key[0] == '-')
    {
         this->channels_line[channel_name].Set_limit(0);
         Broadcast_to_the_channel(channel_name, RAW_MODE(this->client_line[fd].get_nick(), 
          this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, key, std::string("")));
        return ;
    }
    if (limit.empty())
    {
        send_message(fd, std::string("461 MODE :Not enough parameters"));
        return ;
    }
    else if (limit_int <= 0)
    {
        send_message(fd, ERR_INVALID_MODE_PARAM(channel_name, limit));
        return ;
    }
    this->channels_line[channel_name].Set_limit(limit_int);
    Broadcast_to_the_channel(channel_name, RAW_MODE(this->client_line[fd].get_nick(), this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, key, limit));


}

void Server::apply_mode(int fd, Message msg, char key)
{
    if ((this->mode_command_options).find(key) != this->mode_command_options.end())
        (this->*(this->mode_command_options[key]))(fd, msg);
}

int Server::check_mod_arg(int fd, Message msg)
{
    std::string channel_name = Server::trim_white(msg.params[0]); 
    std::string key  =  (msg.params.size() > 1) ? Server::trim_white(msg.params[1]) : std::string("");

    if (this->channels_line.find(channel_name) == this->channels_line.end())
    {
        send_message(fd, ERR_NOSUCHCHANNEL(channel_name));	
        return (0);
    }
    else if (!this->channels_line[channel_name].Get_operators(fd))
    {
        send_message(fd, ERR_CHANOPRIVSNEEDED(channel_name));
        return (0);   
    }
    else if (key.empty() || !is_valid_mode_key(key))
    { 
        std::cout << "key == " << key << std::endl;
        send_message(fd, ERR_UNKNOWNMODE(key, channel_name));	
        return (0);
    }
    return (1);
}

void Server::command_mode(int fd, Message msg)
{
    if (!check_mod_arg(fd, msg))
        return ;
    else if (msg.params.size() > 1)
        apply_mode(fd, msg, msg.params[1][1]);
    else
        return ;
}