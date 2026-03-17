/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:32:56 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 15:08:13 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

    Client::Client():AHost(-1), registered(0)
    {
        
        ft_memset(this->receive_bytes_buffer, 0,513);
        ft_memset(this->sent_bytes_buffer, 0, 513);
        // ft_memset(this->receive_line, 0, 513);
        ft_memset(this->sent_line, 0, 513);
        // std::cout << "Constructor client called " << std::endl;
    }

    Client::~Client()
    {
        // std::cout << "Destructor client called with fd == "<< this->fd_socket << std::endl;
        if (this->fd_socket != -1)
            close(this->fd_socket);

    }

    Client::Client(int fd) : AHost(fd), registered(0)
    {
        // std::cout << "Other Constructor client called " << std::endl;
        ft_memset(this->receive_bytes_buffer, 0,513);
        ft_memset(this->sent_bytes_buffer, 0, 513);
        // ft_memset(this->receive_line, 0, 513);
        ft_memset(this->sent_line, 0, 513);
    }

    Client::Client(const Client & copy) : AHost(copy)
    {
        //  std::cout << "Copy Constructor client called " << std::endl;
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
    
    void Client::set_pass(std::string pass)
    {
        this->pass = pass;
    }

    std::string Client::get_pass()
    {
        return (this->pass);
    }

    void Client::set_nick(std::string nick)
    {
        this->nickname = nick;
    }

    std::string Client::get_nick()
    {
        return (this->nickname);
    }
    void Client::set_username(std::string username)
    {
        this->username = username;
    }

    std::string Client::get_username()
    {
        return (this->username);
    }

    void Client::set_realname(std::string realname)
    {
        this->realname = realname;
    }

    std::string Client::get_realname()
    {
        return (this->realname);
    }
    void Client::set_servername(std::string servername)
    {
        this->servername = servername;
    }

    std::string Client::get_servername()
    {
        return (this->servername);
    }

    void Client::set_IP_adress(std::string IP)
    {
        this->IP_address = IP;

    }

    std::string Client::get_IP_adress()
    {   
        return (this->IP_address);
    }

    void Client::Init_connection(void)
    {
        
    } 