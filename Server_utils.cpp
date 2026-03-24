/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngogang <ngogang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 16:47:46 by ngogang           #+#    #+#             */
/*   Updated: 2026/03/14 18:00:21 by ngogang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHost.hpp"
#include "Channel.hpp"
#include "Server.hpp"

std::string Server::trim_white(std::string str)
{
    std::string::iterator ite;
    if (str.begin() == str.end())
        return (str);
    ite = --str.end();
    while (*ite <= 32 && str.size())
        str.erase(ite--);
    return (str);
}

bool	Server::isCharCorrect(char c)
{
	return (isalnum(c) || c == '-' || c == '[' || c == ']' ||
		c == '\\' || c == '`' || c == '^' || c == '{' || c == '}');
}

int Server::is_a_valid_nickname(std::string nick)
{
    if (nick.size() > 9)
        return (0);
    for (std::string::iterator it = nick.begin(); it != nick.end(); ++it)
    {
        if (!isCharCorrect(*it))
            return(0);
    }
    return (1);
}

int Server::is_a_valid_channel_name(std::string channel_name)
{
    std::string::iterator it = channel_name.begin();
    if (channel_name.size() > 50)
        return (0);
    if (*it != '#' && *it != '&')
        return (0);
     ++it;
    for (; it != channel_name.end(); ++it)
    {
        if (*it < 33)
            return (0);
        else if (*it == ':')
            return (0);
        else if (*it == ',')
            return(0);
    }
    return (1);
}

int Server::is_a_channel(std::string str)
{
    if (str[0] == '&' || str[0] == '#')
        return (1);
    return (0);
}

std::vector<std::string> Server::split_string(std::string str, char delimiter)
{
    std::stringstream ss(str);
    std::string buffer;
    std::vector<std::string> result;
    if (str.find(delimiter) == std::string::npos)
    {
        result.push_back(Server::trim_white(str));
        return (result);
    }
    while (std::getline(ss, buffer, delimiter)) {
        result.push_back(Server::trim_white(buffer));
    }

    return result;
}

void Server::add_channel_client_to_string(Channel & channel, std::string & msg)
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
