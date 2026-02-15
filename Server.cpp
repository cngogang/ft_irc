/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:47:07 by ngogang           #+#    #+#             */
/*   Updated: 2026/02/14 18:37:16 by ngogang          ###   ########.fr       */
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

//replace strlen by ft_strlen 
void Server::add_fd_to_epoll_interest_list(int fd_epoll, int fd_to_add)
{
    struct epoll_event client_ev;
    
    client_ev.events = EPOLLIN | EPOLLOUT;
    client_ev.data.fd = fd_to_add;

    // epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &client_ev);
    // this->epoll_handler_struct.events = EPOLLIN | EPOLLET;
    // this->epoll_handler_struct.data.fd = fd_to_add;
    std::cout << "fd to add " << fd_to_add << " at epoll fd : " << fd_epoll << std::endl;
    // if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_to_add, &(this->epoll_handler_struct)) == -1)
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_to_add, &(client_ev)) == -1)
        throw EpollCtlError();
    
}

void Server::create_a_new_client_and_add_it_to_interest_list()
{
    Client new_connexion;

    int accept_return_fd = accept(this->fd_socket, (struct sockaddr *)(new_connexion.get_sockaddr_in6()), &new_connexion.size_addr);
    new_connexion.set_fd_socket(accept_return_fd);
    std::cout << "NEW CLIENT FD : " << accept_return_fd << std::endl; 
    this->make_socket_non_blocking(accept_return_fd);
    add_fd_to_epoll_interest_list(this->epoll_fd, accept_return_fd);
    this->client_line[accept_return_fd] = new_connexion;
    std::cout << "map value " << this->client_line[accept_return_fd].get_fd_socket() << std::endl;
    // sleep(2);
    // this->handle_request(accept_return_fd);
    
    // while (new_connexion.byte_read > 0)
    // {
    //     new_connexion.byte_read = recv(new_connexion.AHost::get_fd_socket(), new_connexion.receive_data, 50, 0);
    //     new_connexion.receive_data[new_connexion.byte_read] = '\0';
    //     if (!new_connexion.byte_read)
    //         std::cout << "connexion close " << std::endl;
    //     else if (new_connexion.byte_read == -1)
    //         std::cout << "connexion error " << std::endl;
    //     else
    //         {
    //             std::cout << "Client sent : " << new_connexion.receive_data << std::endl;
                
    //             new_connexion.byte_sent = send(new_connexion.AHost::get_fd_socket(), "Server sent : ", 14, 0);
    //             if (new_connexion.byte_sent == -1)
    //             {
    //                 std::cout << "Send failure .." << std::endl;
    //                 break ;
    //             }
    //             get_server_response_form_stdin(new_connexion.sent_data);
    //             new_connexion.byte_sent = send(new_connexion.AHost::get_fd_socket(), new_connexion.sent_data, strlen(new_connexion.sent_data), 0);
    //             if (new_connexion.byte_sent == -1)
    //             {
    //                 std::cout << "Send failure .." << std::endl;
    //                 break ;
    //             }
    //         }
    // }



}

void Server::handle_request(int fd)
{
    // Client *current_client = &(this->client_line[fd]);
    // std::string exit("exit");
    // current_client->byte_read = 1;
    // std::cout << "HR FD "<< current_client->AHost::get_fd_socket() << std::endl;
    // current_client->byte_read = recv(current_client->AHost::get_fd_socket(), current_client->receive_data, 50, 0);
    // // current_client->receive_data[current_client->byte_read] = '\0';
    // if(current_client->byte_read == -1)
    //     std::cout << "FD "<< fd << "Nothing to read for the moment" << std::endl;
    // else
    //     std::cout << "FD "<< fd <<  " : " << current_client->receive_data << std::endl;
    // while (exit != current_client->receive_data || exit != current_client->sent_data )
    // {
    //     memset(current_client->receive_data, 0, strlen(current_client->receive_data));
    //     current_client->byte_read = recv(current_client->AHost::get_fd_socket(), current_client->receive_data, 50, 0);
    //     std::cout << "recv return " << current_client->byte_read << std::endl;

    //     if (current_client->byte_read != -1)
    //         current_client->receive_data[current_client->byte_read] = '\0';
    //     else if (current_client->byte_read == -1)
    //         std::cout << "waiting for something " << std::endl;
    //     else
    //         {
    //             // std::cout << "recv return " << current_client->byte_read << std::endl;
    //             std::cout << "fd " << fd << " sent : " << current_client->receive_data << std::endl;
                
    //             // current_client->byte_sent = send(current_client->AHost::get_fd_socket(), "Server sent : ", 14, 0);
    //             memset(current_client->receive_data, 0, strlen(current_client->receive_data));
    //             get_server_response_form_stdin(current_client->sent_data);
    //             current_client->byte_sent = send(current_client->AHost::get_fd_socket(), current_client->sent_data, strlen(current_client->sent_data), 0);

    //         }
    // }
}

void Server::Listen_loop()
{
    struct epoll_event client_event[3000];
    int fd_ready = 0;
    int map[1025]; 
    int current_fd_ready;
    if (listen(this->fd_socket, 10) != 0)
        throw ListenError();
    while(1)
    {
        fd_ready = epoll_wait(this->epoll_fd, client_event, 3000, -1);
        if (fd_ready == -1)
            throw EpollWaitError();
        for(int i = 0; i < fd_ready; ++i)
        {
            std::cout << "FD ready == " << fd_ready << " FD epoll == " << this->epoll_fd << " | client_event fd == "<< client_event[i].data.fd << " | this->fd socket == " << this->fd_socket<<std::endl;
            if (client_event[i].data.fd == -1)
                break ;
            else if (client_event[i].data.fd == this->fd_socket)
                create_a_new_client_and_add_it_to_interest_list();  
            else
                handle_request(client_event[i].data.fd);
                
        }


    }
} 


void Server::Create_epoll_instance()
{
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1)
        throw EpollError();
}

void Server::Add_listening_socket_to_epoll()
{
    int epoll_ctl_status;

    this->epoll_handler_struct.events = EPOLLIN ;
    this->epoll_handler_struct.data.fd = this->fd_socket;
    epoll_ctl_status = epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->fd_socket, &(this->epoll_handler_struct));
    if (epoll_ctl_status == -1)
        throw EpollError();
}


void Server::Create_epoll_instance_and_bind_server_socket()
{
    Create_epoll_instance();
    Add_listening_socket_to_epoll();

}

void Server::Listen_and_handle_request()
{
    Create_epoll_instance_and_bind_server_socket();
    Listen_loop();
}



// if (listen(this->fd_socket, 10) == 0)
// {
//     client.AHost::set_fd_socket(accept(this->fd_socket, (struct sockaddr *)(client.AHost::get_sockaddr_in6()), &client.AHost::size_addr));
//     if (client.AHost::get_fd_socket() == 0)
//         throw SocketError();
//     client.byte_read = 1;
//     client.receive_data[1024] = '\0';
//     while (client.byte_read > 0)
//     {
//         client.byte_read = recv(client.AHost::get_fd_socket(), client.receive_data, 50, 0);
//         client.receive_data[client.byte_read] = '\0';
//         if (!client.byte_read)
//             std::cout << "connexion close " << std::endl;
//         else if (client.byte_read == -1)
//             std::cout << "connexion error " << std::endl;
//         else
//             {
//                 std::cout << "Client sent : " << client.receive_data << std::endl;
                
//                 client.byte_sent = send(client.AHost::get_fd_socket(), "Server sent : ", 14, 0);
//                 if (client.byte_sent == -1)
//                 {
//                     std::cout << "Send failure .." << std::endl;
//                     break ;
//                 }
//                 get_server_response_form_stdin(client.sent_data);
//                 client.byte_sent = send(client.AHost::get_fd_socket(), client.sent_data, strlen(client.sent_data), 0);
//                 if (client.byte_sent == -1)
//                 {
//                     std::cout << "Send failure .." << std::endl;
//                     break ;
//                 }
//             }
//     }

//     return ;

void Server::enable_IPv4_connexion()
{
    int v6only = 0;

    if (setsockopt(this->fd_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) < 0)
        std::cerr << "setsockopt IPV6_V6ONLY" << std::endl;
    return ;
    
}
void Server::make_socket_non_blocking(int fd)
{
    int original_bitmask_flags_fd;
    original_bitmask_flags_fd = fcntl(fd, F_GETFL, 0);
    original_bitmask_flags_fd = fcntl(fd, F_SETFL, original_bitmask_flags_fd & O_NONBLOCK);
    // original_bitmask_flags_fd = fcntl(fd, F_SETFL, O_NONBLOCK);
}
void Server::bind_socket()
{
    int bind_status;
    
    bind_status = bind(this->fd_socket, (struct sockaddr *)(&(this->connexion_info_v6)), sizeof(this->connexion_info_v6));
    if (bind_status != 0)
        throw BindError();
    else 
        std::cout << "socket bind on port : "  << ntohs(this->connexion_info_v6.sin6_port) << std::endl;
}

void Server::Open_socket()
{
    int bind_status = 0;

    this->fd_socket = socket(this->connexion_info_v6.sin6_family, SOCK_STREAM, 0);
    if (this->fd_socket == 0)
        throw SocketError(); 
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
    Set_connexion_v6();
}

void Server::Init_connection()
{
    Set_connexion();
    Open_socket();
    enable_IPv4_connexion();
    make_socket_non_blocking(this->fd_socket);
    bind_socket();
}
void Server::Run()
{
    try
    {
        Init_connection();
        Listen_and_handle_request();

    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << errno << std::endl;
    }
}
