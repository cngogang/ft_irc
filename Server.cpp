/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:47:07 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/08 19:09:49 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Server.hpp"
#include "Client.hpp"


Server::Server():port(8080)
{
    Run();
}
Server::Server(int num_port):port(num_port) 
{
    Run();
}
Server::~Server()
{
    shutdown(this->fd_socket, SHUT_RDWR);
    close(this->fd_socket);
}
Server::Server(const Server & copy)
{
    if(this != &copy)
        *this = copy;
}
Server & Server::operator=(const Server & rightOperand)
{
    (void)rightOperand;
    return (*this);
}
void Server::Handle_response()
{
    
}

void Server::get_server_response_form_stdin(char *msg)
{
    std::string str;
    std::getline(std::cin, str);
    int i = 0;
    for (std::string::iterator it = str.begin(); it != str.end() && std::distance(str.begin(), it) < 1023; ++it, ++i)
        msg[i] = *it;
    msg[i] = '\n';
    msg[i] = '\0';
}


void Server::Listen_request()
{
    Client client;

if (listen(this->fd_socket, 10) == 0)
{
    client.AHost::set_fd_socket(accept(this->fd_socket, (struct sockaddr *)(client.AHost::get_sockaddr_in6()), &client.AHost::size_addr));
    if (client.AHost::get_fd_socket() == 0)
        throw SocketError();
    client.byte_read = 1;
    client.receive_data[1024] = '\0';
    while (client.byte_read > 0)
    {
        client.byte_read = recv(client.AHost::get_fd_socket(), client.receive_data, 50, 0);
        client.receive_data[client.byte_read] = '\0';
        if (!client.byte_read)
            std::cout << "connexion close " << std::endl;
        else if (client.byte_read == -1)
            std::cout << "connexion error " << std::endl;
        else
            {
                std::cout << "Client sent : " << client.receive_data << std::endl;
                
                client.byte_sent = send(client.AHost::get_fd_socket(), "Server sent : ", 14, 0);
                if (client.byte_sent == -1)
                {
                    std::cout << "Send failure .." << std::endl;
                    break ;
                }
                get_server_response_form_stdin(client.sent_data);
                client.byte_sent = send(client.AHost::get_fd_socket(), client.sent_data, strlen(client.sent_data), 0);
                if (client.byte_sent == -1)
                {
                    std::cout << "Send failure .." << std::endl;
                    break ;
                }
            }
    }

    return ;
}
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
    int bind_status = 0;

    this->fd_socket = socket(this->connexion_info_v6.sin6_family, SOCK_STREAM, 0);
    if (this->fd_socket == 0)
        throw SocketError();
    enable_IPv4_connexion();
    bind_status = bind(this->fd_socket, (struct sockaddr *)(&(this->connexion_info_v6)), sizeof(this->connexion_info_v6));
    if (bind_status != 0)
        throw BindError();
    else 
        std::cout << "socket bind on port : "  << ntohs(this->connexion_info_v6.sin6_port) << std::endl; 
}

void Server::Set_connexion_v4()
{
    // this->connexion_info_v4.sin_family = AF_INET;
    // this->connexion_info_v4.sin_addr.s_addr = htonl(INADDR_ANY);
    // this->connexion_info_v4.sin_port = htons(80);
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
    // Set_connexion_v4();
    Set_connexion_v6();
}
void Server::Init_connection()
{
    Set_connexion();
    Open_socket();
}
void Server::Run()
{
    try
    {
        Init_connection();
        Listen_request();

    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << errno << std::endl;
    }
}
