/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:47:07 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/04 21:43:28 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"


Server::Server():port(8080)
{
    Run();
}
Server::Server(int num_port, std::string pass):port(num_port), password(pass) 
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
    for (std::string::iterator it = str.begin(); it != str.end() && std::distance(str.begin(), it) < 512; ++it, ++i)
        msg[i] = *it;
    msg[i] = '\n';
    msg[i] = '\0';
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

void Server::create_a_new_client_and_add_it_to_interest_list()
{
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof(client_addr);   
    int accept_return_fd = accept(this->fd_socket, (struct sockaddr *)&client_addr, &addr_len);

    this->client_line[accept_return_fd];
    (this->client_line[accept_return_fd]).set_fd_socket(accept_return_fd);
    (this->client_line[accept_return_fd]).set_IP_adress(get_incoming_connexion_ip(client_addr));
    this->make_socket_non_blocking(accept_return_fd);
    add_fd_to_epoll_interest_list(this->epoll_fd, accept_return_fd, EPOLLIN | EPOLLET);
}

// void Server::read_bytes_and_build_message(Client current_client)
// {

// }


// static int is_no_new_line(char *buffer_512_bytes)
// {
//     int i = 0;

//     for ( ;buffer_512_bytes[i]; ++i)
//     {
//         if (buffer_512_bytes[i] == '\n')
//             return (0);
//     }
//     if (i == 1)
//         return (0);
//     return (1);

// }

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
    // for (std::string::iterator it = raw_message.begin(); it <= end_command_name ; ++it)
    // {
    //     raw_message.erase(it);
    // }
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
        if (start_arg == raw_message.end() || start_arg == colon_position)
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
    
    if (colon_position != raw_message.end())
        ++colon_position;
    
    msg.trailing_params.assign(colon_position, raw_message.end());
}

// static void extract_command_prefix(std::string & raw_message, Message & msg)
// {
//     std::string prefix;
//     std::string::iterator first_space;
//     std::string::iterator e_mark;
//     std::string::iterator at;
// ➜  ft_irc_git git:(main)
//     if (*(raw_message.begin()) != ':')
//         return ;
//     first_space = std::find(raw_message.begin(), raw_message.end(), ' ');
//     if (first_space == raw_message.end())
//         return ;
//     prefix.assign(raw_message.begin(),  first_space);
//     e_mark = std::find(prefix.begin(), prefix.end(), '!');
//     if (e_mark == prefix.end())
//         return ;
//     at = std::find(prefix.begin(), prefix.end(), '!');
//     if (at == prefix.end() || at < e_mark)
//         return ;
//     msg.prefix = prefix;
//     for (std::string::iterator it = raw_message.begin(); it <= first_space ; ++it)
//     {
//         raw_message.erase(it);
//     }
    
// }

void Server::build_message_object_and_proceed_it(Client & current_client, Message & msg)
{
    std::string::iterator end_c_name;
    std::string::iterator colon_position;
    std::string::iterator start_arg;
    std::string::iterator end_arg;
    std::string raw_message(current_client.receive_line);
    std::string param_buff;

    std::cout << "HERE 1" << std::endl;
    // end_command_name = std::find(raw_message.begin(), raw_message.end(), ' ');
    // msg.command.assign(raw_message.begin(), end_command_name);
    ft_memset(current_client.receive_line, 0, strlen(current_client.receive_line));
    
    // extract_command_prefix(raw_message, msg);
    // if (msg.prefix.empty())
    //     return ;
    extract_command_name(raw_message, msg); 
        std::cout << "HERE 2" << std::endl;
    if (msg.command.empty())
         return ;
    extract_command_arg(raw_message, msg);
        std::cout << "HERE 3" << std::endl;
    if (msg.params.end() == msg.params.begin())
         return ;
    extract_trailing_param(raw_message, msg);
        std::cout << "HERE 4" << std::endl;
    if ((this->commands).find(msg.command) != this->commands.end())
        (this->*(this->commands[msg.command]))(current_client.get_fd_socket(), msg);
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
    std::cout << "NAME COMMAND : " << msg.command << "|" << std::endl;
    for (std::vector<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        std::cout << "ARG COMMAND : " << (*it) << "|" << std::endl;
    }
    std::cout << "trailing param : " << msg.trailing_params <<"|" << std::endl;
}


    // std::cout << "NAME COMMAND : " << msg.command << std::endl;
    // for (std::vector<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    // {
    //     std::cout << "ARG COMMAND : " << (*it) << std::endl;
    // }
    // std::cout << "trailing param : " << msg.trailing_params << std::endl;
    // AHost::ft_memset(current_client->sent_bytes_buffer, 0, strlen(current_client->sent_bytes_buffer));
    // get_server_response_form_stdin(current_client->sent_bytes_buffer);
    // current_client->byte_sent = send(current_client->AHost::get_fd_socket(), current_client->sent_bytes_buffer, strlen(current_client->sent_bytes_buffer), 0);


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
        // std::cout << "Befor loop stdin check " << fcntl(0, F_GETFL, 0) << std::endl;
        for(int i = 0; i < fd_ready; ++i)
        {
            // std::cout << "In listen loop stdin check " << fcntl(0, F_GETFL, 0) << std::endl;
            if (client_event[i].data.fd == -1)
                break ;
            else if (client_event[i].data.fd == this->fd_socket)
                create_a_new_client_and_add_it_to_interest_list();  
            else
                handle_request(client_event[i].data.fd);    
        }
        //  std::cout << "after listen loop stdin check " << fcntl(0, F_GETFL, 0) << std::endl;

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
    // std::cout << "Original bitmask : " << original_bitmask_flags_fd << std::endl;
    // std::cout << "1 stdin check " << fcntl(0, F_GETFL, 0) << std::endl;


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
    if (this->fd_socket == -1)
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
        Init_command_map();
        Listen_and_handle_request();

    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << errno << std::endl;
    }
}

void Server::Init_command_map()
{
    this->commands["NICK"] = &Server::command_nick;
    this->commands["PASS"] = &Server::command_pass;
    this->commands["USER"] = &Server::command_user;
    this->commands["PRIVMSG"] = &Server::command_priv_msg;
    this->commands["JOIN"] = &Server::command_join;
    this->commands["PART"] = &Server::command_join;
    this->commands["NAMES"] = &Server::command_names;

}
int Server::is_register(int fd)
{
    
    std::cout << "IR NICK : " << this->client_line[fd].get_nick() << std::endl;
    std::cout << "IR USER : " << this->client_line[fd].get_username() << std::endl;
    std::cout << "IR PASS client : " << this->client_line[fd].get_pass() << " server : " << this->password << " bool == " << (this->password == this->client_line[fd].get_pass())  << std::endl;
    if (this->client_line[fd].get_nick().empty() || this->client_line[fd].get_username().empty())
        return (0);
    if (this->client_line[fd].get_pass() != this->password)
        return (0);
    return (1);

}


void Server::send_message(const int & recipient_fd, const std::string & msg)
{
    int byte_sent;
    
    if (msg.size())
    byte_sent = send(recipient_fd, msg.c_str(), msg.size(), 0);

    if (byte_sent == -1)
        throw sendError();
}

static void extract_prefix(std::string & msg, std::string & prefix, std::string::iterator  *start)
{
    std::string::iterator first_colon;
    std::string::iterator second_colon;

    first_colon = std::find(msg.begin(), msg.end(), ':');
    if (first_colon == msg.end())
        return ;
    second_colon = std::find(first_colon + 1, msg.end(), ':');
    if (msg.end() == second_colon)
        return ;
    if (start)
        *start = second_colon + 1;
    prefix.assign(first_colon, second_colon + 1);
}

void Server::split_msg_and_send_it(const int & recipient_fd,  std::string & msg)
{
    std::string prefix;
    int body_size;
    std::string buffer;
    std::string splited_msg;
    std::string::iterator start;
    std::string::iterator end;
    std::string::iterator temp;
    

    extract_prefix(msg, prefix, &start);
    body_size = 510 - static_cast<int>(prefix.size());
    end = std::find(start, msg.end(), ' ');

    while (start != msg.end())
    {
        while ( end - start <= body_size)
        {
          temp = end;
          if (end == msg.end())
            break ;
          end = std::find(end + 1, msg.end(), ' ');
        }
        buffer.clear();
        splited_msg.clear();
        buffer.assign(start, temp);
        splited_msg = prefix + buffer + "\r\n";
        send_message(recipient_fd, splited_msg);
        if (temp == msg.end())
            break ;
        start = temp + 1;
        
    }


    



}

// static void CRLF_end_and_send_message(const int & recipient_fd,  std::string & msg)
// {
//     if (msg.find("\r\n") == std::string::npos && msg.find("\n") == )
//     {
//         msg.push_back('\r');
//         msg.push_back('\n');
//     }
//     if (msg.size() > 512)
//         split_msg_and_send_it(recipient_fd,msg);
// }

void Server::build_prefix_and_send_message(const int & sender_fd, const int & recipient_fd, const Message & receiveid_message)
{
    
    std::string msg_to_send;
    std::string prefix;

    prefix = ":" + this->client_line[sender_fd].get_nick() + "!" + this->client_line[sender_fd].get_username() + "@" + this->client_line[sender_fd].get_IP_adress();
    msg_to_send = prefix + " " + receiveid_message.command + " " + receiveid_message.params[0] + " :" + receiveid_message.trailing_params;
    send_message(recipient_fd, msg_to_send);

}

void Server::welcome_msg(int fd)
{
    std::string line_1("001 Welcome message\r\n");
    std::string line_2("002 ft_IRC\r\n");
    std::string line_3("003 Created date\r\n");
    std::string line_4("004 Server info\r\n");


    send_message( fd, line_1);
    send_message( fd, line_2);
    send_message( fd, line_3);
    send_message( fd, line_4);
}

static std::string trim_CRLF(std::string str)
{
    std::string::iterator ite;
    ite = --str.end();
    while (*ite == '\n' || *ite == '\r' )
        str.erase(ite--);
    return (str);
}
void Server::command_pass(int fd, Message msg)
{
    std::string pass;

    if (msg.params.size() != 1)
    {
        std::cerr << "Wrong number of argument : COMMAND PASS." << std::endl;
        return ;
    }
    pass = trim_CRLF(msg.params[0]);
    this->client_line[fd].set_pass(pass);
    if (is_register(fd))
        welcome_msg(fd);
}
void Server::command_nick(int fd, Message msg)
{
    std::string nick;

    if (msg.params.size() != 1)
    {
        std::cerr << "Wrong number of argument : COMMAND NICK." << std::endl;
        return ;
    }
    else if (this->client_line_by_nick.find(msg.params[0]) != this->client_line_by_nick.end())
    {
        std::cerr << "Nickname already exist : COMMAND NICK ." << std::endl;
        return ;
    }
    nick = trim_CRLF(msg.params[0]);
    this->client_line[fd].set_nick(nick);
    this->client_line_by_nick[nick] = &(this->client_line[fd]);
    if (is_register(fd))
        welcome_msg(fd);
}


void Server::command_user(int fd, Message msg)
{
    std::string username;
    
    if (msg.params.size() != 1)
    {
        std::cerr << "Wrong number of argument : COMMAND USER." << std::endl;
        return ;
    }
    username = trim_CRLF(msg.params[0]);
    this->client_line[fd].set_username(username);
    if (is_register(fd))
        welcome_msg(fd);
}
static std::string trim_white(std::string str)
{
    std::string::iterator ite;
    ite = --str.end();
    while (*ite <= 32 )
        str.erase(ite--);
    return (str);
}

void Server::send_message_to_channel(const int & fd, const Message & msg)
{
    std::string channel_name = trim_white(msg.params[0]);
    std::map<int, Client *>::iterator it;
    Channel target_channel;
    std::map<int, Client *> target_channel_members;
    std::map<int, Client *> target_channel_operators;
    std::cout << "send msg channel > "<< msg.params[0] << std::endl;
    if (this->channels_line.find(channel_name) == this->channels_line.end())
        return ;
    std::cout << "send msg channel" << std::endl;
    target_channel = this->channels_line[channel_name];
    target_channel_members = target_channel.Get_members();
    target_channel_operators = target_channel.Get_operators();
    for(it = target_channel_operators.begin();it != target_channel_operators.end() ; ++it)
    {
        build_prefix_and_send_message(fd, (*it).first,  msg);
    } 
    for(it = target_channel_members.begin();it != target_channel_members.end() ; ++it)
    {
        build_prefix_and_send_message(fd, (*it).first,  msg);
    } 
}

void Server::send_message_to_client(const int & fd, const Message & msg)
{
    std::string client_nick_name = msg.params[0];

    if (this->client_line_by_nick.find(client_nick_name) == this->client_line_by_nick.end())
        return ;
    build_prefix_and_send_message(fd, this->client_line_by_nick[client_nick_name]->get_fd_socket(), msg);
}



static int is_a_channel(std::string str)
{
    std::cout << "is_achennel == " << str << " / str[0] == " << str[0] << std::endl;
    if (str[0] == '&' || str[0] == '#')
        return (1);
    return (0);
}

void Server::command_priv_msg(int fd, Message msg)
{
    std::cout << "HERE cmd" << std::endl;
    if (!is_register(fd))
        return ;
    if (is_a_channel(msg.params[0]))
        send_message_to_channel(fd, msg);
    else
        send_message_to_client(fd, msg);
}


void Server::join_channel(int client_fd, std::string channel_name)
{
    std::string channel_name_trim = trim_white(channel_name);
    if (!is_a_channel(channel_name_trim))
        return ;
    std::cout << "join channel 2" << std::endl;
    if (this->channels_line.find(channel_name_trim) == this->channels_line.end())
    {
         std::cout << "creation channel "<< channel_name_trim << std::endl; 
        this->channels_line[channel_name_trim] =  Channel(this->client_line[client_fd], client_fd, channel_name_trim);
        this->client_line[client_fd].Channel_list.push_back(channel_name_trim);
    }
    else
        {
            std::cout << "add member channel "<< channel_name_trim  << std::endl;
            this->channels_line[channel_name_trim].add_members(this->client_line[client_fd], client_fd);
            this->client_line[client_fd].Channel_list.push_back(channel_name_trim);
        }
}
void Server::command_join(int fd, Message msg)
{
    std::cout << "command join " << std::endl;
    for (std::vector<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        std::cout << "command join loop " << *it << std::endl;
        join_channel(fd, *it);
    }   
}

void Server::part_channel(int client_fd, std::string channel_name)
{
    std::string channel_name_trim = trim_white(channel_name);
    std::vector<std::string>::iterator channel_name_pos =  std::find(this->client_line[client_fd].Channel_list.begin(), this->client_line[client_fd].Channel_list.begin(), channel_name_trim); 
    if (!is_a_channel(channel_name_trim))
        return ;
    std::cout << "join channel 2" << std::endl;
    if (this->channels_line.find(channel_name_trim) == this->channels_line.end() || channel_name_pos == this->client_line[client_fd].Channel_list.end())
        return ;
    else
        {
            std::cout << "add member channel "<< channel_name_trim  << std::endl;
            this->channels_line[channel_name_trim].remove_members(client_fd);
            this->client_line[client_fd].Channel_list.erase(channel_name_pos);
        }
}

void Server::command_part(int fd, Message msg)
{
    for (std::vector<std::string>::iterator it = msg.params.begin(); it != msg.params.end(); ++it)
    {
        part_channel(fd, *it);
    }   
}

static void add_channel_client_to_string(Channel & channel, std::string & msg)
{
    for (std::map<int, Client *>::iterator it = channel.Get_operators().begin(); it != channel.Get_operators().end(); ++it)
    {
        msg += " @" + ((*it).second)->get_nick();
    }
    for (std::map<int, Client *>::iterator it = channel.Get_members().begin(); it != channel.Get_members().end(); ++it)
    {
        msg += " " + ((*it).second)->get_nick();
    }
}


void Server::command_names(int fd, Message msg)
{
    std::vector<std::string> list_channel;
    std::string command_response = ":ft_irc 353 " + this->client_line[fd].get_nick();
    if (msg.params.begin() == msg.params.end())
    {
        list_channel = this->client_line[fd].Channel_list;
        for (std::map<std::string, Channel>::iterator it; it != this->channels_line.end(); ++it)
        {
            if (!(*it).second.is_private() && std::find(list_channel.begin(), list_channel.end(), (*it).first) == list_channel.end())
                list_channel.push_back((*it).first);
        }
    }
    else
        list_channel = msg.params;
    
    for (std::vector<std::string>::iterator it = list_channel.begin(); it != list_channel.begin(); ++it)
    {
        if (this->channels_line[*it].is_private()) 
            command_response += " *" + this->channels_line[*it].Get_name();
        else
            command_response += " =" + this->channels_line[*it].Get_name();
        add_channel_client_to_string(this->channels_line[*it], command_response);
    }
    split_msg_and_send_it(fd,  command_response);
}
