/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc_format.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cngogang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:19:40 by cngogang          #+#    #+#             */
/*   Updated: 2026/03/09 13:19:55 by cngogang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# define SUCCESS	"\e[32m[v] "
# define INFO		"\e[36m[i] "
# define WARNING	"\e[35m[!] "
# define ERROR		"\e[31m[x] "
# define MSG		"\e[33m[#] "
# define RESET		"\e[0m"

// ========================================================================== //
//     MISC                                                                   //
// ========================================================================== //

# define ERR_UNKNOWNCOMMAND(command)	"421 " + command + " :Unknown command\r\n"
# define ERR_NEEDMOREPARAMS(command)	"461 " + command + " :Not enough parameters\r\n"

# define RAW_PRIVMSG(sender, hostname, receiver, message) \
	":" + sender + "!~" + sender + "@" + hostname + " PRIVMSG " + receiver + " :" + message + "\r\n"

# define RAW_BROADCAST(sender, username, hostname, channel, message) \
	":" + sender + "!~" + username + "@" + hostname + " PRIVMSG " + channel + " :" + message + "\r\n"

# define RAW_PART(nickname, username, hostname, channel, reason) \
	":" + nickname + "!~" + username + "@" + hostname + " PART " + channel + " :" + reason + "\r\n"

# define RAW_QUIT(nickname, username, hostname, message) \
	":" + nickname + "!~" + username + "@" + hostname + " QUIT :" + message + "\r\n"

// ========================================================================== //
//     PASS                                                                   //
// ========================================================================== //

# define ERR_NOTREGISTERED		"451 :You have not registered\r\n"
# define ERR_ALREADYREGISTRED	"462 :You may not reregister\r\n"
# define ERR_PASSWDMISMATCH		"464 :Password incorrect\r\n"

// ========================================================================== //
//     NICK                                                                   //
// ========================================================================== //

# define ERR_NONICKNAMEGIVEN			"431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(nickname)	"432 " + nickname + " :Erroneus nickname\r\n"
# define ERR_NICKNAMEINUSE(nickname)	"433 " + nickname + " :Nickname is already in use\r\n"

# define RAW_NICKNAME(oldNick, newNick, username, hostname)	\
	":" + oldNick + "!" + username + "@" + hostname + " NICK :" + newNick + "\r\n"

// ========================================================================== //
//     JOIN                                                                   //
// ========================================================================== //

# define ERR_CHANNELISFULL(channel)		"471 " + channel + " :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLYCHAN(channel)	"473 " + channel + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(channel)		"475 " + channel + " :Cannot join channel (+k)\r\n"
# define ERR_BADCHANMASK(channel)		"476 " + channel + " :Bad Channel Mask\r\n"

# define RAW_JOIN(nickname, username, hostname, channel) \
	":" + nickname + "!~" + username + "@" + hostname + " JOIN :" + channel + "\r\n"

# define RPL_NAMREPLY(target, channel, names) \
	":ft_irc 353 " + target + " = " + channel + " :" + names + "\r\n"

# define RPL_ENDOFNAMES(target, channel) \
	":ft_irc 366 " + target + " " + channel + " :End of /NAMES list.\r\n"

// ========================================================================== //
//     MODE                                                                   //
// ========================================================================== //

# define ERR_NOSUCHNICK(nickname)		"401 " + nickname + " :No such nick/channel\r\n"
# define ERR_NOSUCHCHANNEL(channel)		"403 " + channel + " :No such channel\r\n"
# define ERR_UNKNOWNMODE(arg, channel)	("472 " + std::string(1, arg) + " :is unknown mode char to me for " + channel + "\r\n")
# define ERR_CHANOPRIVSNEEDED(channel)	"482 " + channel + " :You're not channel operator\r\n"

# define RAW_MODE_ADDOP(sender, username, hostname, channel, target) \
    (":" + sender + "!~" + username + "@" + hostname + " MODE " + channel + " +o " + target + "\r\n")

# define RAW_MODE_REMOP(sender, username, hostname, channel, target) \
    (":" + sender + "!~" + username + "@" + hostname + " MODE " + channel + " -o " + target + "\r\n")

# define RAW_MODE(nickname, username, hostname, channel, mode, arg) \
	":" + nickname + "!~" + username + "@" + hostname + " MODE " + channel + " " + mode + (arg.empty() ? "" : " " + arg) + "\r\n"

// ========================================================================== //
//     TOPIC                                                                  //
// ========================================================================== //

# define RPL_NOTOPIC(nickname, channel) ":ft_irc 331 " + nickname + " " + channel + " :No topic is set\r\n"

# define RAW_TOPIC(nickname, username, hostname, channel, topic) \
	":" + nickname + "!~" + username + "@" + hostname + " TOPIC " + channel + " :" + topic + "\r\n"

# define RPL_TOPIC(nickname, channel, topic) \
	":ft_irc 332 " + nickname + " " + channel + " :" + topic + "\r\n"

// ========================================================================== //
//     INVITE                                                                 //
// ========================================================================== //

# define RPL_INVITING(inviter, invited, channel) \
    ":ft_irc 341 " + std::string(inviter) + " " + std::string(invited) + " " + std::string(channel) + "\r\n"

# define INVITE_MESSAGE(inviter, invited, channel) \
    ":" + std::string(inviter) + "!~" + inviter + "@host INVITE " + std::string(invited) + " :" + std::string(channel) + "\r\n"

// ========================================================================== //
//     KICK                                                                   //
// ========================================================================== //

# define RPL_KICK(kicker, target, channel, comment) \
    ":" + std::string(kicker) + "!~" + std::string(kicker) + "@host KICK " + std::string(channel) + " " + std::string(target) + " :" + std::string(comment) + "\r\n"

# define ERR_NTONCHANNEL(nickname, channel) \
	"442 " + nickname + " " + channel + " :You're not on that channel\r\n" 

# define ERR_USERNOTINCHANNEL(kicker, target, channel) \
	":ft_irc 441 " + kicker + " " + target + " " + channel + " :They aren't on that channel\r\n"    
	