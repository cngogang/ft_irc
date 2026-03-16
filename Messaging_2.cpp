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
// #include "Server_utils.cpp"

static int copy_bytes(char *src, char *dst)
{
    int j = 0;
    while (dst[j])
        ++j;
    int i = 0;
    while (src[i] && (i + j) < 512)
    {
        dst[i + j] = src[i];
        ++i;
    }
    if (i <= 512)
    {
        dst[i + j] = '\0';
        return (i);
    }    
    else
        return (i);
}
int Server::line_is_CRLF_termininated(const Client & current_client)
{
    int i;
    for (i = 0; current_client.receive_line[i]; ++i)
    {   }
    
    if (current_client.receive_line[i - 1] == '\n')
        return (1);
    return (0);    
}

int Server::receive_bytes(Client  & current_client)
{
    int returned_bytes;


    returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);   
    AHost::ft_memset(current_client.receive_bytes_buffer, 0, 513);
    if (returned_bytes > 512)
    {
        AHost::ft_memset(current_client.receive_bytes_buffer, 0, 513);
        AHost::ft_memset(current_client.receive_line, 0, 513);
        return (-1);
    }
    // AHost::ft_memset(current_client.receive_bytes_buffer, 0, strlen(current_client.receive_bytes_buffer));
    current_client.byte_read = recv(current_client.AHost::get_fd_socket(), current_client.receive_bytes_buffer, 512, 0);
    // returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);

    return (0);
}


static void extract_command_name( std::string & raw_message, Message & msg)
{
    std::string::iterator end_command_name;
    std::cout << "ECN" << std::endl; 
    end_command_name = std::find(raw_message.begin(), raw_message.end(), ' ');
    msg.command.assign(raw_message.begin(), end_command_name);
    msg.command = Server::trim_white(msg.command);
    // for (std::string::iterator it = raw_message.begin(); it <= end_command_name ; ++it)
    // {
    //     raw_message.erase(it);
    // }
}

static void extract_command_arg( std::string & raw_message, Message & msg)
{
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string param_buff;

    std::cout << "ECA" << std::endl;
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
    std::cout << "ETP" << std::endl;
    colon_position = std::find(raw_message.begin(), raw_message.end(), ':');
    
    if (colon_position != raw_message.end())
        ++colon_position;
    
    buff.assign(colon_position, raw_message.end());
    msg.trailing_params = Server::trim_white(buff);
}

// static void extract_command_prefix(std::string & raw_message, Message & msg)
// {
//     std::string prefix;
//     std::string::iterator first_space;
//     std::string::iterator e_mark;
//     std::string::iterator at;
// ➜  ft_irc_git git:(main)
//     if (*(raw_message.begin()) != ':')
//         return ;
//     first_space = std::find(raw_message.begin(), raw_message.end(), ' ');
//     if (first_space == raw_message.end())
//         return ;
//     prefix.assign(raw_message.begin(),  first_space);
//     e_mark = std::find(prefix.begin(), prefix.end(), '!');
//     if (e_mark == prefix.end())
//         return ;
//     at = std::find(prefix.begin(), prefix.end(), '!');
//     if (at == prefix.end() || at < e_mark)
//         return ;
//     msg.prefix = prefix;
//     for (std::string::iterator it = raw_message.begin(); it <= first_space ; ++it)
//     {
//         raw_message.erase(it);
//     }
    
// }

void Server::build_message_object_and_proceed_it(Client & current_client, Message & msg)
{
    std::string::iterator end_c_name;
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string raw_message(current_client.receive_line);
    std::string param_buff;

    ft_memset(current_client.receive_line, 0, 513);
    extract_command_name(raw_message, msg); 
    if (msg.command.empty())
         return ;
    extract_command_arg(raw_message, msg);
    extract_trailing_param(raw_message, msg);

    std::cout << "HERE" << std::endl;
    std::cout << "HERE : " << msg.command << std::endl;
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
