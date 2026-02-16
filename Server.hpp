/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:38:13 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/12 20:55:14 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AHost.hpp"
#include <map>
#include <algorithm>

class Client;

class Server : public AHost
{
    public: 
    Server();
    Server(int num_port);
    ~Server();
    Server(const Server & copy);
    Server & operator=(const Server & rightOperand);
    void Run();
   
    private:
    void get_server_response_form_stdin(char *msg);
    void Set_connexion();
    void bind_socket();
    void Open_socket();
    void Listen_and_handle_request();
    void Handle_response();
    void memset_param_connexion();
    void Init_connection();
    // void Set_connexion_v4();
    void Set_connexion_v6();
    void enable_IPv4_connexion();
    void make_socket_non_blocking(int fd);
    void Create_epoll_instance();
    void Add_listening_socket_to_epoll();
    void Listen_loop();
    void create_a_new_client_and_add_it_to_interest_list();
    void add_fd_to_epoll_interest_list(int fd_epoll, int fd_to_add, int flag);
    void handle_request(int fd);
    void Create_epoll_instance_and_bind_server_socket();
    std::map<int, Client> client_line;
    int port;
    int epoll_fd;
    

};