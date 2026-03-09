/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:02 by cngogang          #+#    #+#             */
/*   Updated: 2026/03/04 21:35:14 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"


    Channel::Channel():invit_only_mode(0)
    {

    }
    Channel::~Channel()
    {

    }

    Channel::Channel( Client & first_member, const int & first_member_fd, const std::string channel_name): name(channel_name) , invit_only_mode(0)
    {
      this->operators[first_member_fd] = &first_member;

    }


    Channel::Channel(const Channel & copy)
    {
        if(this != &copy)
          *this = copy;

    }


    void Channel::add_members(Client & first_member,  const int & first_member_fd)
    {
        if (!this->invit_only_mode)
            this->members[first_member_fd] = &first_member;
        else if (std::find(this->invitation_list.begin(), this->invitation_list.end(), first_member_fd) != this->invitation_list.end())
            this->members[first_member_fd] = &first_member;
        return ;
    }
    // void Channel::remove_operators(Client & member,  int & member_fd)
    // {
    //     std::map<int, Client *>::iterator member_list_pos = this->members.find(member_fd);
    //     std::map<int, Client *>::iterator operator_list_pos = this->operators.find(member_fd);


    //     if (member_list_pos != this->members.end())
    //         this->members.erase(member_list_pos);
    //     if (operator_list_pos != this->operators.end())
    //        {
               
    //     } this->operators.erase(operator_list_pos);
    // }

    void Channel::remove_members(int & member_fd)
    {
        std::map<int, Client *>::iterator member_list_pos = this->members.find(member_fd);
        std::map<int, Client *>::iterator operator_list_pos = this->operators.find(member_fd);


        if (member_list_pos != this->members.end())
            this->members.erase(member_list_pos);
        else if (operator_list_pos != this->operators.end())
        {
             this->operators.erase(operator_list_pos);
             if (this->operators.empty() && !this->members.empty())
             {        
                add_operators(*(*this->members.begin()).second, ((*this->members.begin()).first));
             }
        }
    }
     void Channel::add_operators(Client & first_member,  const int & first_member_fd)
    {
         this->operators[first_member_fd] = &first_member;
    }


    Channel & Channel::operator=(const Channel & rightOperand)
    {
        this->topic = rightOperand.topic;
        this->name = rightOperand.name;
        this->invit_only_mode = rightOperand.invit_only_mode;
        this->operators = rightOperand.operators;
        this->members = rightOperand.members;
        this->invitation_list = rightOperand.invitation_list;
        return (*this);

    }
      std::map<int, Client*> &  Channel::Get_members()
    {
        return (this->members);
    }

    const Client *Channel::Get_operators(int fd)
    {
        if (this->operators.find(fd) == this->operators.end())
            return (NULL);
        else
            return (this->operators[fd]);
    } 
    std::map<int, Client*> &  Channel::Get_operators()
    {
        return (this->operators);
    }
    std::string Channel::Get_name()
    {
        return (this->name);
    }

    void Channel::Set_name(std::string str_name)
    {
        this->name = str_name;
    }
    int Channel::is_private()
    {
        return (this->invit_only_mode);
    }