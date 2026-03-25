/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multyplexing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:24:41 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 17:35:26 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int Server::check_errno_value()
{
    std::cout << "check erno" << std::endl;
    if (errno == EAGAIN)
    {
        errno = 0;
        return (0);
    }
    else
        throw recvError();
}

static std::string get_incoming_connexion_ip(struct sockaddr_storage client_addr)
{
    char ip_str[INET6_ADDRSTRLEN];
    std::string IP_adress;
    if (client_addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
        inet_ntop(AF_INET, &s->sin_addr, ip_str, sizeof(ip_str));
    } else {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&client_addr;
        inet_ntop(AF_INET6, &s->sin6_addr, ip_str, sizeof(ip_str));
    }
    IP_adress = ip_str;
 return (IP_adress);
}

void Server::set_client_buffer(Client & client)
{
    ft_memset(client.receive_bytes_static_buffer, 0, sizeof(client.receive_bytes_static_buffer));
    ft_memset(client.sent_bytes_buffer, 0, sizeof(client.sent_bytes_buffer));
    ft_memset(client.sent_line, 0, sizeof(client.sent_line));

}
void Server::create_a_new_client_and_add_it_to_interest_list()
{
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof(client_addr);   
    int accept_return_fd;
    try
    {
        accept_return_fd = accept(this->fd_socket, (struct sockaddr *)&client_addr, &addr_len);
        this->client_line[accept_return_fd];
        (this->client_line[accept_return_fd]).set_fd_socket(accept_return_fd);
        (this->client_line[accept_return_fd]).set_IP_adress(get_incoming_connexion_ip(client_addr));
        
        set_client_buffer(this->client_line[accept_return_fd]);
        
        this->make_socket_non_blocking(accept_return_fd);
        add_fd_to_epoll_interest_list(this->epoll_fd, accept_return_fd, EPOLLIN | EPOLLET);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << errno <<  '\n';
    }
    std::cout << "Connected to " << get_incoming_connexion_ip(client_addr) << "." << std::endl;
}


void Server::handle_request(int fd)
{
    Client *current_client = &(this->client_line[fd]);
    int  bytes_received = 0;
    Message msg;

    msg.fd_issuer = fd;
    AHost::ft_memset(current_client->receive_bytes_static_buffer, 0, 513);
    current_client->byte_read = 1;
    std::cout << "Handling request from " << current_client->get_IP_adress() << "." << std::endl;
    try
    {
        while (current_client->byte_read > 0)
        { 
            bytes_received = receive_bytes(*current_client);
            
            build_message_object_and_proceed_it(*current_client, msg);
            if (current_client->byte_read == -1 && !check_errno_value())
                break ;
            if (current_client->byte_read == 0|| current_client->is_quiting())
            {

                this->client_line_by_nick.erase(this->client_line[fd].get_nick());
                this->client_line.erase(fd);
                std::cout << "Connexion closed with fd : " << fd << std::endl; 
                break ;
            }
        }    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << errno <<  '\n';
    }
    
    
}

void Server::Listen_loop()
{
    struct epoll_event client_event[3000];
    int fd_ready = 0;

    while(Server::is_running)
    {
        fd_ready = epoll_wait(this->epoll_fd, client_event, 3000, -1);
        std::cout << "Listening to port : " << this->port << std::endl;
        if (fd_ready == -1)
            break ;    
        for(int i = 0; i < fd_ready; ++i)
        {
            if (client_event[i].data.fd == -1)
                break ;
            else if (client_event[i].data.fd == this->fd_socket)
                create_a_new_client_and_add_it_to_interest_list();  
            else
                handle_request(client_event[i].data.fd);    
        }
    }
} 



void Server::Listen_and_handle_request()
{
    Listen_loop();
}
