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

// static int copy_bytes(char *src, char *dst)
// {
//     int j = 0;
//     while (dst[j])
//         ++j;
//     int i = 0;
//     while (src[i] && (i + j) < 512 )
//     {
//         dst[i + j] = src[i];
//         ++i;
//     }

//     if (i <= 512)
//     {
//         dst[i + j] = '\0';
//         return (i);
//     }    
//     else
//         return (i);
// // }
// int Server::line_is_CRLF_termininated(const Client & current_client)
// {
//     int i;
//     for (i = 0; current_client.receive_line[i]; ++i)
//     {   }
    
//     if (current_client.receive_line[i - 1] == '\n')
//         return (1);
//     return (0);    
// }

int Server::receive_bytes(Client  & current_client)
{
    
    std::string bytes_received;

    // returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);   
    current_client.byte_read = recv(current_client.AHost::get_fd_socket(), current_client.receive_bytes_buffer, 512, 0);
    // if (current_client.byte_read == - 1)
    //     throw AHost::recvError();
    bytes_received = std::string(current_client.receive_bytes_buffer);
    current_client.receive_line += bytes_received;
    AHost::ft_memset(current_client.receive_bytes_buffer, 0, 513);
    // if (returned_bytes > 512)
    // {
    //     AHost::ft_memset(current_client.receive_bytes_buffer, 0, 513);
    //     AHost::ft_memset(current_client.receive_line, 0, 513);
    //     return (-1);
    // }
    // AHost::ft_memset(current_client.receive_bytes_buffer, 0, strlen(current_client.receive_bytes_buffer));
    // returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);

    return (0);
}


static void extract_command_name( std::string & raw_message, Message & msg)
{
    std::string::iterator end_command_name;
    //  std::cout << "ECN : " << raw_message << "end ECN" << std::endl;
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

    // std::cout << "ECA : " << raw_message << "end ECA" << std::endl;
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
    //  std::cout << "ETP : " << raw_message << "end ETP" << std::endl;
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

void extract_line_from_receive_line(std::string &receive_line, std::string & line)
{
    std::string::iterator nl_pos;
    std::string buffer;

    std::cout << "BEFORE r_l = " << receive_line << std::endl;
    std::cout << "bEFORE l = " << line << std::endl;
    nl_pos = std::find(receive_line.begin(),receive_line.end(), '\n');
    if (nl_pos == receive_line.begin())
        return ;
    buffer.assign(nl_pos + 1, receive_line.end());
    line.assign(receive_line.begin(), nl_pos + 1);
    receive_line = buffer;
    // for (std::string::iterator it = receive_line.begin(); it <= nl_pos ; ++it)
    // {
    //     receive_line.erase(it);
    // }
    std::cout << "AFTER r_l = " << receive_line << std::endl;
    std::cout << "AFTER l = " << line << std::endl;
}

void Server::build_message_object_and_proceed_it(Client & current_client, Message & msg)
{
    std::string::iterator end_c_name;
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string line;
    std::string param_buff;


    // ft_memset(current_client.receive_line, 0, 513);
    while (current_client.receive_line.find('\n') != std::string::npos)
    {
        extract_line_from_receive_line(current_client.receive_line, line);
        extract_command_name(line, msg); 
        if (msg.command.empty())
            return ;
        extract_command_arg(line, msg);
        extract_trailing_param(line, msg);
        std::cout << "COMMAND NAME : " << msg.command << std::endl;
        for (size_t i = 0; i < msg.params.size(); ++i)
            std::cout << "COMMAND PARAM : " << msg.params[i] << std::endl;
        std::cout << "COMMAND TRAILING : " << msg.trailing_params << std::endl;
        proceed_message(current_client, msg);
        msg = Message();
    }
    
    
}
