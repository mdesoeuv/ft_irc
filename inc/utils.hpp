#pragma once

//Error Messages
#define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(client)						"433 " + client + " " + client  + " :Nickname is already in use"
#define ERR_UNKNOWNCOMMAND(client, command)				"421 " + client + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters"
#define ERR_NOTREGISTERED(client)						"451 " + client + " :You have not registered"
#define ERR_ALREADYREGISTERED(client)					"462 " + client + " :You may not reregister"
#define ERR_PASSWDMISMATCH(client)						"464 " + client + " :Password incorrect"
#define ERR_TOOMANYCHANNELS(client, channel)			"405 " + client + " " + channel + " :You have joined too many channels"
#define ERR_NOTONCHANNEL(client, channel)				"442 " + client + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(client, channel)				"403 " + client + " " + channel + " :No such channel"
#define ERR_BADCHANNELKEY(client, channel)				"475 " + client + " " + channel + " :Cannot join channel (+k)"
#define ERR_NOSUCHNICK(client, nickname)				"401 " + client + " " + nickname + " :No such nick/channel"
#define ERR_USERNOTINCHANNEL(client, nickname, channel)	"441 " + client + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_CHANOPRIVSNEEDED(client, channel)			"482 " + client + " " + channel + " :You're not channel operator"
#define ERR_CHANNELISFULL(client, channel)				"471 " + client + " " + channel + " :Cannot join channel (+l)"
#define ERR_CANNOTSENDTOCHAN(client, channel)			"404 " + client + " " + channel + " :Cannot send to channel"

// NUMERIC REPLIES
#define RPL_WELCOME(client)						"001 " + client + " :Welcome " + client + " to the ft_irc network"
#define RPL_NAMREPLY(client, channel, users)	"353 " + client + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(client, channel)			"366 " + client + " " + channel + " :End of /NAMES list."

// COMMAND REPLIES
#define RPL_JOIN(client, channel)					":" + client + " JOIN :" + channel
#define RPL_PART(client, channel)					":" + client + " PART :" + channel
#define RPL_PING(client, token)						":" + client + " PONG :" + token
#define RPL_PRIVMSG(client, target, message)		":" + client + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(client, target, message)			":" + client + " NOTICE " + target + " :" + message
#define RPL_QUIT(client, message)					":" + client + " QUIT :Quit: " + message
#define RPL_KICK(client, channel, target, reason)	":" + client + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(client, channel, modes, args)		":" + client + " MODE " + channel + " " + modes + " " + args
