/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initialization.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:14:57 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:16:40 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::Init_command_map()
{
    this->register_commands["NICK"] = &Server::command_nick;
    this->register_commands["PASS"] = &Server::command_pass;
    this->register_commands["USER"] = &Server::command_user;
    
    this->commands["PRIVMSG"] = &Server::command_priv_msg;
    this->commands["JOIN"] = &Server::command_join;
    this->commands["PART"] = &Server::command_part;
    this->commands["NAMES"] = &Server::command_names;
    this->commands["KICK"] = &Server::command_kick;

}

void Server::make_socket_non_blocking(int fd)
{
    int original_bitmask_flags_fd;
    original_bitmask_flags_fd = fcntl(fd, F_GETFL, 0);
    // std::cout << "Original bitmask : " << original_bitmask_flags_fd << std::endl;
    // std::cout << "1 stdin check " << fcntl(0, F_GETFL, 0) << std::endl;


    if (original_bitmask_flags_fd == -1)
        throw fcntlError();
    if (fcntl(fd, F_SETFL, original_bitmask_flags_fd | O_NONBLOCK))
        throw fcntlError();
}
void Server::bind_socket()
{
    int bind_status;
    
    bind_status = bind(this->fd_socket, (struct sockaddr *)(&(this->connexion_info_v6)), sizeof(this->connexion_info_v6));
    if (bind_status != 0)
        throw BindError();
    else 
        std::cout << "socket bind on port : "  << ntohs(this->connexion_info_v6.sin6_port) << std::endl;
}

void Server::enable_IPv4_connexion()
{
    int v6only = 0;

    if (setsockopt(this->fd_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) < 0)
        std::cerr << "setsockopt IPV6_V6ONLY" << std::endl;
    return ;
    
}

void Server::Open_socket()
{
    this->fd_socket = socket(this->connexion_info_v6.sin6_family, SOCK_STREAM, 0);
    if (this->fd_socket == -1)
        throw SocketError(); 
}

void Server::Set_connexion_v6()
{
    this->connexion_info_v6.sin6_family = AF_INET6;
    this->connexion_info_v6.sin6_addr = in6addr_any;
    this->connexion_info_v6.sin6_port = htons(this->port);
    this->connexion_info_v6.sin6_flowinfo = 0;
    this->connexion_info_v6.sin6_scope_id = 0;
}
    
void Server::Set_connexion()
{
    AHost::memset_param_connexion();
    Set_connexion_v6();
}

void Server::Init_connection()
{
    Set_connexion();
    Open_socket();
    enable_IPv4_connexion();
    make_socket_non_blocking(this->fd_socket);
    bind_socket();
}