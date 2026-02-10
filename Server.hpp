/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:38:13 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/08 18:58:33 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "AHost.hpp"
#include <vector>
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
    void Open_socket();
    void Listen_request();
    void Handle_response();
    void memset_param_connexion();
    void Init_connection();
    void Set_connexion_v4();
    void Set_connexion_v6();
    void enable_IPv4_connexion();
    std::vector<Client> client_line;
    int port;

};