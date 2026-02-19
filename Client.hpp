/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 15:33:00 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/12 18:03:48 by ngogang          ###   ########.fr       */
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
    char receive_data[1025];
    char sent_data[1025];
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
};