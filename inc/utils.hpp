#pragma once

#include <iostream>
#include <string>
#include <vector>

/* replies Message */

#define RPL_WELCOME(myclient)							"001 " + myclient + " :Welcome " + myclient + " to the ft_irc network"
#define RPL_QUIT(myclient, message)						":" + myclient + " QUIT :Quit: " + message
#define RPL_PING(myclient, token)						":" + myclient + " PONG :" + token
#define RPL_CHANNELMODEIS(myclient, mychannel, mode_string, mode_args)	"324 " + myclient + " " + mychannel + " " + mode_string + mode_args
#define RPL_NOTOPIC(myclient, mychannel)				"331 " + myclient + " " + mychannel + " :No topic is set"
#define RPL_TOPIC(myclient, mychannel, channel_topic)	"332 " + myclient + " " + mychannel + " :" + channel_topic
#define RPL_NAMEREPLY(myclient, channel, users)  	 	"353 " + myclient + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(myclient, mychannel)				"366 " + myclient + " " + mychannel + " :End of /NAMES list"

/* Error Messages */

#define ERR_NOSUCHCHANNEL(myclient, mychannel)			"403 " + myclient + " " + mychannel + " :No such channel"
#define ERR_NOTONCHANNEL(myclient, mychannel)			"442 " + myclient + " " + mychannel + " :You're not on that channel"
#define ERR_NONICKNAMEGIVEN(myclient)					"431 " + myclient + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(myclient)				"433 " + myclient + " " + myclient  + " :Nickname is already in use"
#define ERR_CMDNEEDMOREPARAMS(myclient, command)		"461 " + myclient + " " + command + "  :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(myclient, command)			"421 " + myclient + " " + command + " :Unknown command"
#define ERR_ALREADYAUTHENTIFIED(myclient)				"462 " + myclient + " :You may not reregister"
#define ERR_WRONGPASSWORD(myclient)						"464 " + myclient + " :Password incorrect"
#define ERR_ALREADYREGISTERED(myclient)					"462 " + myclient + " :You may not reregister"

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result);
