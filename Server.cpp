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
#include "Channel.hpp"


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

void Server::read_bytes_and_build_message(Client current_client)
{

}


static int is_no_new_line(char *buffer_512_bytes)
{
    int i = 0;

    for ( ;buffer_512_bytes[i]; ++i)
    {
        if (buffer_512_bytes[i] == '\n')
            return (0);
    }
    if (i == 1)
        return (0);
    return (1);

}

static int copy_bytes(char *src, char *dst)
{
    int j = 0;
    while (dst[j])
        ++j;
    int i = 0;
    while (src[i] && (i + j) < 512)
    {
        dst[i + j] = src[i];
        ++i;
    }
    if (i <= 512)
    {
        dst[i + j] = '\0';
        return (i);
    }    
    else
        return (i);
}
int Server::line_is_CRLF_termininated(const Client & current_client)
{
    int i;
    for (i = 0; current_client.receive_line[i]; ++i)
    {   }
    if (current_client.receive_line[i - 1] == '\n')
        return (1);
    return (0);    
}

int Server::receive_bytes(Client  & current_client)
{
    int returned_bytes;
    // if (is_no_new_line(current_client.receive_bytes_buffer))
    returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);   
    AHost::ft_memset(current_client.receive_bytes_buffer, 0, strlen(current_client.receive_bytes_buffer));
    if (returned_bytes > 512)
    {
        AHost::ft_memset(current_client.receive_bytes_buffer, 0, 513);
        AHost::ft_memset(current_client.receive_line, 0, 513);
        return (-1);
    }
    // AHost::ft_memset(current_client.receive_bytes_buffer, 0, strlen(current_client.receive_bytes_buffer));
    current_client.byte_read = recv(current_client.AHost::get_fd_socket(), current_client.receive_bytes_buffer, 512, 0);
    // returned_bytes = copy_bytes(current_client.receive_bytes_buffer, current_client.receive_line);

    return (0);
}







static void extract_command_name( std::string & raw_message, Message & msg)
{
    std::string::iterator end_command_name;

    end_command_name = std::find(raw_message.begin(), raw_message.end(), ' ');
    msg.command.assign(raw_message.begin(), end_command_name);
}

static void extract_command_arg( std::string & raw_message, Message & msg)
{
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string param_buff;

    start_arg = std::find(raw_message.begin(), raw_message.end(), ' ');
    if (start_arg == raw_message.end())
        return ;
    ++start_arg;
    colon_position = std::find(start_arg, raw_message.end(), ':');
    end_arg = std::find(start_arg, raw_message.end(), ' ');
    while (end_arg != raw_message.end() && end_arg < colon_position)
    {
        param_buff.assign(start_arg, end_arg);
        msg.params.push_back(param_buff);
        start_arg = ++end_arg;
        if (start_arg == raw_message.end())
            break ;
        end_arg = std::find(start_arg, raw_message.end(), ' ');
    }
    if (end_arg == raw_message.end())
    {
        param_buff.assign(start_arg, end_arg);
        msg.params.push_back(param_buff);
    }
}

static void extract_trailing_param( std::string & raw_message, Message & msg)
{
    std::string::iterator colon_position;

    colon_position = std::find(raw_message.begin(), raw_message.end(), ':');
    
    msg.trailing_params.assign(colon_position, raw_message.end());
}

void Server::build_message_object_and_proceed_it(const Client & current_client, Message & msg)
{
    std::string::iterator end_c_name;
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string raw_message(current_client.receive_line);
    std::string param_buff;

    // end_command_name = std::find(raw_message.begin(), raw_message.end(), ' ');
    // msg.command.assign(raw_message.begin(), end_command_name);
    extract_command_name(raw_message, msg); 
    if (msg.command.empty())
         return ;
    extract_command_arg(raw_message, msg);
    if (msg.params.end() == msg.params.begin())
         return ;
    extract_trailing_param(raw_message, msg);

    // if (colon_position != raw_message.end())
    //     msg.trailing_params.assign(colon_position, raw_message.end());
}

int Server::check_errno_value(void)
{
    if (errno == EAGAIN)
    {
        errno = 0;
        return (0);
    }
    else
        throw recvError();
}



void Server::handle_request(int fd)
{
    Client *current_client = &(this->client_line[fd]);
    int  bytes_received;
    Message msg;

    msg.fd_issuer = fd;

        
    AHost::ft_memset(current_client->receive_bytes_buffer, 0, strlen(current_client->receive_bytes_buffer));
    current_client->byte_read = 1;

    while (current_client->byte_read != -1)
    { 
        
        bytes_received = receive_bytes(*current_client);
        if (bytes_received > 512)
            return ;
        if (line_is_CRLF_termininated(*current_client))
            build_message_object_and_proceed_it(*current_client, msg);
        if (current_client->byte_read == -1 && !check_errno_value())
            break ;
           
    }
    std::cout << "NAME COMMAND : " << msg.command << std::endl;
    for (std::vector<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        std::cout << "ARG COMMAND : " << (*it) << std::endl;
    }
    std::cout << "trailing param : " << msg.trailing_params << std::endl;
    AHost::ft_memset(current_client->sent_bytes_buffer, 0, strlen(current_client->sent_bytes_buffer));
    get_server_response_form_stdin(current_client->sent_bytes_buffer);
    current_client->byte_sent = send(current_client->AHost::get_fd_socket(), current_client->sent_bytes_buffer, strlen(current_client->sent_bytes_buffer), 0);
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
