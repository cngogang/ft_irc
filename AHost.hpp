/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHost.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:17:09 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/13 18:46:11 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_CLIENT 3000

class AHost
{
    public:
    AHost();
    AHost(int fd);
    virtual ~AHost();
    AHost(const AHost & copy);
    AHost & operator=(const  AHost  & rightOperand);
    void copy_host_connection_param(const struct AHost & RightOperand);
    //  void copy_host_connection_info(const struct AHost & RightOperand);
    static  void addrinfo_memset(struct addrinfo *ai);
    virtual void Init_connection(void) = 0;
    void memset_param_connexion();
    // void set_fd_socket4(int fd);
    // int get_fd_socket4();
    void set_fd_socket(int fd);
    int get_fd_socket() const;
    struct sockaddr_in *get_sockaddr_in();
    struct sockaddr_in6 *get_sockaddr_in6();
    // void copy_host_connection_info();
    class BindError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };
    class SocketError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };

    class EpollError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };

    class ListenError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };

    class EpollWaitError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };
    class EpollCtlError : public std::exception
    {
                public:
                virtual const char *what() const throw();
                // void printError() const throw();
    };
    
    socklen_t size_addr;
    protected:
    struct sockaddr_in connexion_info_v4;
    struct sockaddr_in6 connexion_info_v6;
    struct addrinfo         connection_param;
    int    fd_socket;
    struct epoll_event epoll_handler_struct;


};