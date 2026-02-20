/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:32:56 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/13 18:47:07 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

    Client::Client()
    {
        ft_memset(this->receive_bytes_buffer, 0,513);
        ft_memset(this->sent_bytes_buffer, 0, 513);
        ft_memset(this->receive_line, 0, 513);
        ft_memset(this->sent_line, 0, 513);
    }
    Client::~Client()
    {
        close(this->fd_socket);

    }

    Client::Client(int fd) : AHost(fd)
    {

    }

    Client::Client(const Client & copy)
    {
        if (this != &copy)
        {
            *this = copy;
        } 
    }
    Client & Client::operator=(const Client & rightOperand)
    {
        this->copy_host_connection_param(rightOperand);
        this->set_fd_socket(rightOperand.get_fd_socket());
        return(*this);
        // this->fd_socket = rightOperand.fd_socket;
    }
    
     void Client::Init_connection(void)
     {

     } 