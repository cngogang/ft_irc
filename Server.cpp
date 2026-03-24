/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:47:07 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:54:14 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



int Server::is_running = 0;


void Signal_handling(int sig)
{
    if (sig == SIGINT)
        Server::is_running = 0;
    
}

Server::Server():port(8080)
{
    Run();
}
Server::Server(int num_port, std::string pass):port(num_port), password(pass) 
{
    Run();
}
Server::~Server()
{
    shutdown(this->fd_socket, SHUT_RDWR);
    close(this->fd_socket);
}
Server::Server(const Server & copy) : AHost(copy)
{
    if(this != &copy)
    *this = copy;
}

Server & Server::operator=(const Server & rightOperand)
{
    (void)rightOperand;
    return (*this);
}

void Server::get_server_response_form_stdin(char *msg)
{
    std::string str;
    std::getline(std::cin, str);
    int i = 0;
    for (std::string::iterator it = str.begin(); it != str.end() && std::distance(str.begin(), it) < 512; ++it, ++i)
    msg[i] = *it;
    msg[i] = '\n';
    msg[i] = '\0';
}

void Server::Run()
{
    try
    {
        Init_connection();
        Init_command_map();
        Server::is_running = 1;
        signal(SIGINT, Signal_handling);
        
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << errno << std::endl;
    }
    Listen_and_handle_request();
}

void Server::command_priv_msg(int fd, Message msg)
{
    if (!is_register(fd))
        return ;
    if (is_a_channel(msg.params[0]))
        send_message_to_channel(fd, msg);
    else
        send_message_to_client(fd, msg);
}

int has_white_space(std::string str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (*it <= static_cast<char>(32) || *it == ':')
            return (1);
    }
    return (0);
}




