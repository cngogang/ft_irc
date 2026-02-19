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

void Server::create_a_new_client_and_add_it_to_interest_list()
{
    int accept_return_fd = accept(this->fd_socket, 0, 0);
    this->client_line[accept_return_fd];
    (this->client_line[accept_return_fd]).set_fd_socket(accept_return_fd);
    this->make_socket_non_blocking(accept_return_fd);
    add_fd_to_epoll_interest_list(this->epoll_fd, accept_return_fd, EPOLLIN | EPOLLET);
}

void Server::handle_request(int fd)
{
    Client *current_client = &(this->client_line[fd]);
    Message msg;
    
    if (is_register(*current_client) == -1)
        return ;
    msg = read_bytes_and_build_message(*current_client);
    process_msg(msg);

    AHost::ft_memset(current_client->receive_data, 0, strlen(current_client->receive_data));
    current_client->byte_read = 1;
    int first_loop = 1;
    while (current_client->byte_read != -1)
    { 
        AHost::ft_memset(current_client->receive_data, 0, strlen(current_client->receive_data));
        current_client->byte_read = recv(current_client->AHost::get_fd_socket(), current_client->receive_data, 50, 0);
        if (first_loop)
        {
            first_loop = 0;
        }
        if (current_client->byte_read == -1)
        {
            if (errno == EAGAIN)
                {
                    errno = 0;
                    break ;
                }
            else
                throw recvError();
        }
        std::cout << current_client->receive_data;    
    }
    std::cout << std::endl;
    AHost::ft_memset(current_client->sent_data, 0, strlen(current_client->sent_data));
    get_server_response_form_stdin(current_client->sent_data);
    current_client->byte_sent = send(current_client->AHost::get_fd_socket(), current_client->sent_data, strlen(current_client->sent_data), 0);
}





void Server::Listen_loop()
{
    struct epoll_event client_event[3000];
    int fd_ready = 0;

    if (listen(this->fd_socket, 10) != 0)
        throw ListenError();
    while(1)
    {
        fd_ready = epoll_wait(this->epoll_fd, client_event, 3000, -1);
        if (fd_ready == -1)
            throw EpollWaitError();
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

void Server::add_fd_to_epoll_interest_list(int fd_epoll, int fd_to_add, int flag)
{
    struct epoll_event client_ev;
    
    AHost::ft_memset(&(client_ev), 0, sizeof(client_ev));
    client_ev.events = flag;
    client_ev.data.fd = fd_to_add;
    if (epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_to_add, &(client_ev)) == -1)
        throw EpollCtlError();
    
}

void Server::Create_epoll_instance()
{
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1)
        throw EpollError();
}

void Server::Create_epoll_instance_and_bind_server_socket()
{
    Create_epoll_instance();
    add_fd_to_epoll_interest_list(this->epoll_fd, this->fd_socket, EPOLLIN | EPOLLET);

}

void Server::Listen_and_handle_request()
{
    Create_epoll_instance_and_bind_server_socket();
    Listen_loop();
}


void Server::make_socket_non_blocking(int fd)
{
    int original_bitmask_flags_fd;
    original_bitmask_flags_fd = fcntl(fd, F_GETFL, 0);
    if (original_bitmask_flags_fd == -1)
        throw fcntlError();
    if (fcntl(fd, F_SETFL, original_bitmask_flags_fd | O_NONBLOCK))
        throw fcntlError();
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

void Server::enable_IPv4_connexion()
{
    int v6only = 0;

    if (setsockopt(this->fd_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) < 0)
        std::cerr << "setsockopt IPV6_V6ONLY" << std::endl;
    return ;
    
}

void Server::Open_socket()
{
    this->fd_socket = socket(this->connexion_info_v6.sin6_family, SOCK_STREAM, 0);
    if (this->fd_socket == 0)
        throw SocketError(); 
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
