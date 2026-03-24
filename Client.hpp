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
    void set_servername(std::string servername);
    std::string get_servername();
    void set_IP_adress(std::string IP);
    std::string get_IP_adress();
    void quit();
    int is_quiting();
    char receive_bytes_static_buffer[513];
    std::string receive_bytes_dynamic_buffer;
    char sent_bytes_buffer[513];
    char sent_line[513];
    int fd_to_send;
    int byte_read;
    int byte_sent;
    struct epoll_event *epoll_handler_struct;
    int hasNick;
    int hasUser;
    int registered;
    int want_quit;
    std::vector<std::string> Channel_list;
    // std::map<std::string, Channel>;
    private:
    std::string IP_address;
    std::string nickname;
    std::string username;
    std::string realname;
    std::string servername;
    std::string pass;
};