/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:02 by cngogang          #+#    #+#             */
/*   Updated: 2026/02/22 14:13:03 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"

    Channel::Channel()
    {

    }
    Channel::~Channel()
    {

    }
    Channel::Channel(const Channel & copy)
    {
        if(this != &copy)
          *this = copy;

    }
    Channel & Channel::operator=(const Channel & rightOperand)
    {
        this->topic = rightOperand.topic;
        this->name = rightOperand.name;
        this->invit_only_mode = rightOperand.invit_only_mode;
        this->operators = rightOperand.operators;
        this->members = rightOperand.members;
        return (*this);

    }
    const  std::map<int, Client*> &  Channel::Get_members()
    {
        return (this->members);
    }

    const  std::map<int, Client*> &  Channel::Get_operators()
    {
        return (this->operators);
    }