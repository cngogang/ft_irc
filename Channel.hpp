/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:09 by cngogang          #+#    #+#             */
/*   Updated: 2026/02/22 14:13:08 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include "AHost.hpp"
class Client;

class Channel
{
    public :
    Channel();
    ~Channel();
    Channel(const Channel & copy);
    Channel & operator=(const Channel & copy);
    const  std::map<int, Client*> &  Get_members();
    const  std::map<int, Client*> &  Get_operators();
    
    private :
    std::map<int, Client*> members;
    std::map<int, Client*> operators;
    std::string name;
    std::string topic;
    int invit_only_mode;

};


