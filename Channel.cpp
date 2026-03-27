/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:02 by cngogang          #+#    #+#             */
/*   Updated: 2026/03/15 18:32:32 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"


    Channel::Channel():invit_only_mode(0), topic_restriction(0), limit_user(0)
    {

    }
    Channel::~Channel()
    {

    }

    Channel::Channel( Client & first_member, const int & added_member_fd, const std::string channel_name): invit_only_mode(0), topic_restriction(0), limit_user(0), name(channel_name)
    {
      this->operators[added_member_fd] = &first_member;
      this->host.push_back(added_member_fd);
    }

    Channel::Channel(const Channel & copy)
    {
        if(this != &copy)
          *this = copy;

    }
    void Channel::add_to_invitation_list(int member_fd)
    {
        this->invitation_list.push_back(member_fd);
    }
    void Channel::print_members()
    {
        for (std::map<int, Client*>::iterator it = this->members.begin(); it != this->members.end(); ++it)
        {
            std::cout << "members fd : " << (*it).first << " - addr " << (*it).second << std::endl;
        }
    }

    void Channel::print_operators()
    {
        for (std::map<int, Client*>::iterator it = this->operators.begin(); it != this->operators.end(); ++it)
        {
            std::cout << "OPerator fd : " << (*it).first << " - addr " << (*it).second << std::endl;
        }
    }

    int Channel::add_members(Client & first_member,  const int & added_member_fd)
    {
        std::vector<int>::iterator invitation_list_position = std::find(this->invitation_list.begin(), this->invitation_list.end(), added_member_fd);

        if (!this->invit_only_mode)
        {
            this->members[added_member_fd] = &first_member;
            this->host.push_back(added_member_fd);
        } 
        else if (invitation_list_position != this->invitation_list.end())
        {
            this->members[added_member_fd] = &first_member;
            this->host.push_back(added_member_fd);
            this->invitation_list.erase(invitation_list_position);
        }
        else    
            return (-1);
        return(0);
    }
    
    int Channel::get_size()
    {
        return (this->host.size());
    }
    void Channel::remove_members(int & member_fd)
    {
        std::map<int, Client *>::iterator member_list_pos = this->members.find(member_fd);
        std::map<int, Client *>::iterator operator_list_pos = this->operators.find(member_fd);
        std::vector<int>::iterator host_to_delete;
        int fd_new_operator;
        Client *new_operator;

        if (member_list_pos != this->members.end())
        {
                host_to_delete = std::find(this->host.begin(), this->host.end(),((*member_list_pos).first));
                this->members.erase(member_list_pos);
               this->host.erase(host_to_delete); 
               std::cout << "ERASE MEMBERS" << std::endl;       
        }
        else if (operator_list_pos != this->operators.end())
        {
            host_to_delete = std::find(this->host.begin(), this->host.end(),((*operator_list_pos).first));
            this->operators.erase(operator_list_pos);
            this->host.erase(host_to_delete);
            std::cout << "ERASE OPERATOR" << std::endl;
            if (this->operators.empty() && !this->members.empty())
            {        
                fd_new_operator = *(this->host.begin());
                new_operator = this->members[fd_new_operator];
                add_operators(*new_operator, fd_new_operator);
                remove_members(fd_new_operator);
            }
        }
    }
    
     void Channel::add_operators(Client & first_member,  const int & added_member_fd)
    {
         this->operators[added_member_fd] = &first_member;
         this->host.push_back(first_member.get_fd_socket());
    }

    Channel & Channel::operator=(const Channel & rightOperand)
    {
        this->topic = rightOperand.topic;
        this->name = rightOperand.name;
        this->invit_only_mode = rightOperand.invit_only_mode;
        this->operators = rightOperand.operators;
        this->members = rightOperand.members;
        this->invitation_list = rightOperand.invitation_list;
        this->limit_user = rightOperand.limit_user;
        this->key = rightOperand.key;
        this->topic_restriction = rightOperand.topic_restriction;

        return (*this);

    }
    
    const Client *Channel::Get_members(int fd)
    {
        this->print_members();
        if (this->members.find(fd) == this->members.end())
            return (NULL);
        else
            return (this->members[fd]);
    } 

    std::string Channel::Get_topic()
    {
        return (this->topic);
    }
    void Channel::Set_topic(std::string new_topic)
    {
        this->topic = new_topic;
    }

    std::map<int, Client*> &  Channel::Get_members()
    {
        return (this->members);
    }

    const Client *Channel::Get_operators(int fd)
    {
        this->print_operators();
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

    int Channel::is_in_the_channel(int fd)
    {
        for(std::vector<int>::iterator it = this->host.begin(); it != this->host.end(); ++it)
        {
            if (*it == fd)
                return (1);
        }
        return (0);
    }
    int  Channel::is_topic_restriction()
    {
        return (this->topic_restriction);
    }

    int Channel::is_limited()
    {
        return (this->limit_user);
    }
    int Channel::is_locked()
    {
        return (!this->key.empty());
    }

    std::string Channel::Get_key()
    {
        return (this->key);
    }
    void Channel::Set_limit(int limit)
    {
        this->limit_user = limit;
    }
    void Channel::Set_key(std::string new_key)
    {
        this->key = new_key;
    }
    void Channel::Put_on_invite_only()
    {
        if(!this->invit_only_mode)
            this->invit_only_mode = 1;
    }
    void Channel::Put_off_invite_only()
    {
        if(this->invit_only_mode)
            this->invit_only_mode = 0;
    }

    void Channel::Put_on_topic_privilege()
    {
        if(!this->topic_restriction)
            this->topic_restriction = 1;
    }
    void Channel::Put_off_topic_privilege()
    {
        if(this->topic_restriction)
            this->topic_restriction = 0;
    }
