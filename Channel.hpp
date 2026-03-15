/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:09 by cngogang          #+#    #+#             */
/*   Updated: 2026/03/15 18:25:16 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include "AHost.hpp"
#include "Client.hpp"


class Client;

class Channel
{
    public :
    Channel();
    ~Channel();
    Channel(const Channel & copy);
    Channel(Client & first_member, const int & first_member_fd, const std::string channel_name);
    Channel & operator=(const Channel & copy);
    std::map<int, Client*> &  Get_members();
    std::map<int, Client*> &  Get_operators();
    int add_members(Client & first_member,  const int & first_member_fd);
    void add_operators(Client & first_member, const int & first_member_fd);
    void remove_members( int & member_fd);
    void add_to_invitation_list(int member_fd);
    int is_private();
    int get_size();
    void Set_topic(std::string new_topic);
    std::string Get_topic();
    std::string Get_name();
    void Set_name(std::string str_name);
    const Client *Get_operators(int fd);
    const Client *Get_members(int fd);
    private :
    int invit_only_mode;
    int topic_restriction;
    int has_key;
    int has_limit;
    int limit_user;
    std::vector<int> host;
    std::vector<int> invitation_list;
    std::map<int, Client*> operators;
    std::map<int, Client*> members; 
    std::string name;
    std::string topic;
    std::string key;

    // int size;

};


