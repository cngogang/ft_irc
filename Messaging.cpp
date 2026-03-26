/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messaging.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:48:34 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:50:58 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



void Server::send_message(const int & recipient_fd,  std::string msg)
{
    int byte_sent = 0;
    std::string irc_format_msg;

    if (msg.find('\n') == std::string::npos)
        irc_format_msg = msg + "\r\n";
    else
        irc_format_msg = msg;
    if (msg.size() > 512)
        split_msg_and_send_it(recipient_fd, msg);
    else if (msg.size())
        byte_sent = send(recipient_fd, msg.c_str(), msg.size(), 0);
    if (byte_sent == -1)
        throw sendError();
}

static void extract_prefix(std::string & msg, std::string & prefix, std::string::iterator  *start)
{
    std::string::iterator first_colon;
    std::string::iterator second_colon;

    first_colon = std::find(msg.begin(), msg.end(), ':');
    if (first_colon == msg.end())
        return ;
    second_colon = std::find(first_colon + 1, msg.end(), ':');
    if (msg.end() == second_colon)
        return ;
    if (start)
        *start = second_colon + 1;
    prefix.assign(first_colon, second_colon + 1);
}

void Server::split_msg_and_send_it(const int & recipient_fd, std::string  msg)
{
    std::string prefix;
    int body_size;
    std::string buffer;
    std::string splited_msg;
    std::string::iterator start;
    std::string::iterator end;
    std::string::iterator temp;
    
    std::cout << msg << std::endl;
    extract_prefix(msg, prefix, &start);
    body_size = 510 - static_cast<int>(prefix.size());
    end = std::find(start, msg.end(), ' ');
    while (start != msg.end())
    {
        while ( end - start <= body_size)
        {
          temp = end;
          if (end == msg.end())
            break ;
          end = std::find(end + 1, msg.end(), ' ');
        }
        buffer.clear();
        splited_msg.clear();
        buffer.assign(start, temp);
        splited_msg = prefix + buffer + "\r\n";
        send_message(recipient_fd, splited_msg);
        if (temp == msg.end())
            break ;
        start = temp + 1;
        
    }
}

void Server::send_message_to_channel(const int & fd, const Message & msg)
{
    std::string channel_name = trim_white(msg.params[0]);
    std::map<int, Client *>::iterator it;
    Channel target_channel;
    std::map<int, Client *> target_channel_members;
    std::map<int, Client *> target_channel_operators;

    if (this->channels_line.find(channel_name) == this->channels_line.end() || msg.trailing_params.empty())
        return ;
    if (!this->channels_line[channel_name].is_in_the_channel(fd))
    {
        send_message(fd, ERR_NTONCHANNEL(this->client_line[fd].get_nick(), channel_name));
        return ;
    }
    target_channel = this->channels_line[channel_name];
    target_channel_members = target_channel.Get_members();
    target_channel_operators = target_channel.Get_operators();
    for(it = target_channel_operators.begin();it != target_channel_operators.end() ; ++it)
    {
        if (fd != (*it).first)
            send_message((*it).first, RAW_BROADCAST(this->client_line[fd].get_nick(),this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, msg.trailing_params));
    } 
    for(it = target_channel_members.begin();it != target_channel_members.end() ; ++it)
    {
        if (fd != (*it).first)
            send_message((*it).first, RAW_BROADCAST(this->client_line[fd].get_nick(),this->client_line[fd].get_username(), this->client_line[fd].get_IP_adress(), channel_name, msg.trailing_params));
    } 
} 


void Server::send_message_to_client(const int & fd, const Message & msg)
{
    std::string client_nick_name = trim_white(msg.params[0]);
    std::string sender;
    std::string sender_IP;
    std::string receiver;
    std::string raw_msg;

    if (this->client_line_by_nick.find(client_nick_name) == this->client_line_by_nick.end())
        return ;
    raw_msg = Server::trim_white(msg.trailing_params);
    if (raw_msg.empty())
        return ;
    sender = this->client_line[fd].get_nick();
    sender_IP = this->client_line[fd].get_IP_adress();
    receiver =  this->client_line_by_nick[client_nick_name]->get_nick();
    send_message(this->client_line_by_nick[client_nick_name]->get_fd_socket(), RAW_PRIVMSG(sender, sender_IP, receiver,raw_msg));
}

void Server::build_prefix_and_send_message(const int & sender_fd, const int & recipient_fd, const Message & receiveid_message)
{
    
    std::string msg_to_send;
    std::string prefix;

    prefix = ":" + this->client_line[sender_fd].get_nick() + "!" + this->client_line[sender_fd].get_username() + "@" + this->client_line[sender_fd].get_IP_adress();
    msg_to_send = prefix + " " + receiveid_message.command + " " + receiveid_message.params[0] + " :" + receiveid_message.trailing_params;
    send_message(recipient_fd, msg_to_send);

}

void Server::welcome_msg(int fd)
{
    Client &client = this->client_line[fd];
    char buff[64];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", t);


    std::string line_1(":Welcome to the 42 Network, " + client.get_nick() + "[!" + client.get_username() + "@" + client.get_IP_adress() +"]\r\n");
    std::string line_2("Your host is ircserv, running 1.0 version\r\n");
    std::string line_3("This server was created at " + std::string(buff) +   "\r\n");
    std::string line_4("Command: NAMES, JOIN, PRIVMSG, PART, QUIT, MODE +/- ltoki\r\n");


    send_message( fd, line_1);
    send_message( fd, line_2);
    send_message( fd, line_3);
    send_message( fd, line_4);
}
