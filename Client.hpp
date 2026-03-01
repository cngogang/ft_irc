/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:33:00 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/22 15:16:24 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AHost.hpp"

class Client : public AHost
{
    public:

    Client();
    ~Client();
    Client(int fd);
    Client(const Client & copy);
    Client & operator=(const Client & rightOperand);
    struct sockaddr_storage client_addr;
    void Init_connection(void); 
    void set_pass(std::string pass);
    std::string get_pass();
    void set_nick(std::string nick);
    std::string get_nick();
    void set_username(std::string username);
    std::string get_username();
    void set_realname(std::string realname);
    std::string get_realname();
    char receive_bytes_buffer[513];
    char receive_line[513];
    char sent_bytes_buffer[513];
    char sent_line[513];
    int fd_to_send;
    int byte_read;
    int byte_sent;
    struct epoll_event *epoll_handler_struct;
    int hasNick;
    int hasUser;
    int registered;
    // std::map<std::string, Channel>;
    private:
    std::string nickname;
    std::string username;
    std::string realname;
    std::string pass;
};