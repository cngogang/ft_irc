/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messaging_2.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:34:24 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:56:49 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int Server::receive_bytes(Client  & current_client)
{
    
    std::string bytes_received;

    current_client.byte_read = recv(current_client.AHost::get_fd_socket(), current_client.receive_bytes_static_buffer, 512, 0);
    bytes_received = std::string(current_client.receive_bytes_static_buffer);
    current_client.receive_bytes_dynamic_buffer += bytes_received;
    AHost::ft_memset(current_client.receive_bytes_static_buffer, 0, 513);
    return (0);
}


static void extract_command_name( std::string & raw_message, Message & msg)
{
    std::string::iterator end_command_name;

    end_command_name = std::find(raw_message.begin(), raw_message.end(), ' ');
    msg.command.assign(raw_message.begin(), end_command_name);
    msg.command = Server::trim_white(msg.command);
}

static void extract_command_arg( std::string & raw_message, Message & msg)
{
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string param_buff;

    start_arg = std::find(raw_message.begin(), raw_message.end(), ' ');
    if (start_arg == raw_message.end() || start_arg == raw_message.begin())
        return ;
    ++start_arg;
    colon_position = std::find(start_arg, raw_message.end(), ':');
    while (1)
    {
        end_arg = std::find(start_arg, raw_message.end(), ' ');
        param_buff.assign(start_arg, end_arg);
        msg.params.push_back(Server::trim_white(param_buff));
        start_arg = end_arg + 1;
        if (end_arg == raw_message.end() || end_arg == colon_position - 1)
            break ;   
    }
}

static void extract_trailing_param( std::string & raw_message, Message & msg)
{
    std::string::iterator colon_position;
    std::string buff;

    colon_position = std::find(raw_message.begin(), raw_message.end(), ':');
    if (colon_position != raw_message.end())
        ++colon_position;
    buff.assign(colon_position, raw_message.end());
    msg.trailing_params = Server::trim_white(buff);
}


void Server::proceed_message(Client & current_client, Message & msg)
{
    if ((this->register_commands).find(msg.command) != this->register_commands.end())
        (this->*(this->register_commands[msg.command]))(current_client.get_fd_socket(), msg);
    else if ((this->commands).find(msg.command) != this->commands.end())
    {
        if (is_register(current_client.get_fd_socket()))
            (this->*(this->commands[msg.command]))(current_client.get_fd_socket(), msg);
        else
            send_message(current_client.get_fd_socket(), ERR_NOTREGISTERED);
    }
    else if (msg.command == "CAP")
        return ;
    else
        send_message(current_client.get_fd_socket(),ERR_UNKNOWNCOMMAND(msg.command));
}

void extract_line_from_receive_bytes_dynamic_buffer(std::string &receive_bytes_dynamic_buffer, std::string & line)
{
    std::string::iterator nl_pos;
    std::string buffer;

    nl_pos = std::find(receive_bytes_dynamic_buffer.begin(),receive_bytes_dynamic_buffer.end(), '\n');
    line.assign(receive_bytes_dynamic_buffer.begin(), nl_pos + 1);
    if (nl_pos == receive_bytes_dynamic_buffer.end())
        return ;
    if (receive_bytes_dynamic_buffer.size() == 1)
    {
        line.assign(receive_bytes_dynamic_buffer.begin(), nl_pos + 1);
        receive_bytes_dynamic_buffer = std::string("");
        return ;        
    }
    buffer.assign(nl_pos + 1, receive_bytes_dynamic_buffer.end());
    line.assign(receive_bytes_dynamic_buffer.begin(), nl_pos + 1);
    receive_bytes_dynamic_buffer = buffer;
}

void Server::build_message_object_and_proceed_it(Client & current_client, Message & msg)
{

    std::string line;
    std::string param_buff;

    while (current_client.receive_bytes_dynamic_buffer.find('\n') != std::string::npos)
    {
        extract_line_from_receive_bytes_dynamic_buffer(current_client.receive_bytes_dynamic_buffer, line);
        extract_command_name(line, msg); 
        extract_command_arg(line, msg);
        extract_trailing_param(line, msg);
        proceed_message(current_client, msg);
        msg = Message();
    }
    
    
}
