/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cngogang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 16:41:09 by cngogang          #+#    #+#             */
/*   Updated: 2026/02/19 16:41:10 by cngogang         ###   ########.fr       */
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
    

    private :
    std::string name;
    std::string topic;
    int invit_only_mode;
    std::map<int, Client*> members;
    std::map<int, Client*> operators;

};


