/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHost.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:16:38 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/13 18:47:37 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHost.hpp"

AHost::AHost():size_addr(sizeof(struct sockaddr_in6))
{


}
AHost::AHost(int fd) : fd_socket(fd)
{

}
AHost::~AHost()
{

}
AHost::AHost(const AHost & copy)
{
    if (this != &copy)
        *this = copy;
        
}

AHost & AHost::operator=(const  AHost  & rightOperand)
{
    this->fd_socket = rightOperand.fd_socket;

    return (*this);
}
void AHost::memset_param_connexion()
{
       this->connexion_info_v4.sin_family = 0;
       this->connexion_info_v4.sin_port = 0;
       this->connexion_info_v4.sin_addr.s_addr = 0;

       this->connexion_info_v6.sin6_family = 0;
       this->connexion_info_v6.sin6_port = 0;
       this->connexion_info_v6.sin6_flowinfo = 0;
       this->connexion_info_v6.sin6_scope_id = 0;
       for (int i = 0; i < 16; ++i)
       {
        this->connexion_info_v6.sin6_addr.s6_addr[i] = 0;
       }
}
void AHost::addrinfo_memset(struct addrinfo *ai)
{
    ai->ai_flags = 0;
    ai->ai_family = 0;
    ai->ai_socktype = 0;
    ai->ai_protocol = 0;
    ai->ai_addrlen = 0;
    ai->ai_addr = nullptr;
    ai->ai_canonname = nullptr;
    ai->ai_next = nullptr;
}
const char *AHost::BindError::what() const throw()
{
        return ("Bind issue number :");
}

const char *AHost::SocketError::what() const throw()
{
        return ("Socket issue number :");
}

const char *AHost::EpollError::what() const throw()
{
        return ("Epoll issue number :");
}

const char *AHost::EpollWaitError::what() const throw()
{
        return ("Epoll Wait issue number :");
}

const char *AHost::ListenError::what() const throw()
{
        return ("Listen issue number :");
}

const char *AHost::EpollCtlError::what() const throw()
{
        return ("Epoll Control issue number :");
}


// void AHost::copy_host_connection_info(const struct AHost & RightOperand)
// {
//    this->connexion_info_v6.ai_flags = RightOperand.connection_info.ai_flags; 
//    this->connexion_info_v6.ai_family = RightOperand.connection_info.ai_family; 
//    this->connexion_info_v6.ai_socktype = RightOperand.connection_info.ai_socktype; 
//    this->connexion_info_v6.ai_protocol = RightOperand.connection_info.ai_protocol; 
//    this->connexion_info_v6.ai_addrlen = RightOperand.connection_info.ai_addrlen; 
//    this->connexion_info_v6.ai_addr = RightOperand.connection_info.ai_addr; 
//    this->connection_info.ai_canonname = RightOperand.connection_info.ai_canonname; 
//    this->connection_info.ai_next = RightOperand.connection_info.ai_next; 
// }

// void AHost::set_fd_socket4(int fd)
// {
//  this->fd_socket = fd;   
// }

// int AHost::get_fd_socket4()
// {
//     return (this->fd_socket);
// }

void AHost::set_fd_socket(int fd)
{
    this->fd_socket = fd;
}

int AHost::get_fd_socket() const
{
    return (this->fd_socket );
}

struct sockaddr_in * AHost::get_sockaddr_in()
{
    return (&(this->connexion_info_v4));
}

struct sockaddr_in6 * AHost::get_sockaddr_in6()
{
    return (&(this->connexion_info_v6));
}

void AHost::copy_host_connection_param(const struct AHost & RightOperand)
{
   this->connection_param.ai_flags = RightOperand.connection_param.ai_flags; 
   this->connection_param.ai_family = RightOperand.connection_param.ai_family; 
   this->connection_param.ai_socktype = RightOperand.connection_param.ai_socktype; 
   this->connection_param.ai_protocol = RightOperand.connection_param.ai_protocol; 
   this->connection_param.ai_addrlen = RightOperand.connection_param.ai_addrlen; 
   this->connection_param.ai_addr = RightOperand.connection_param.ai_addr; 
   this->connection_param.ai_canonname = RightOperand.connection_param.ai_canonname; 
   this->connection_param.ai_next = RightOperand.connection_param.ai_next; 
}