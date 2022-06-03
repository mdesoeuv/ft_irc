#pragma once

#include <iostream>
#include <string>
#include <vector>

/* replies Message */

#define RPL_WELCOME(source)							"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_QUIT(myclient, message)					":" + myclient + " QUIT :Quit: " + message
#define RPL_PING(myclient, token)						":" + myclient + " PONG :" + token

/* Error Messages */

#define ERR_NONICKNAMEGIVEN(myclient)					"431 " + myclient + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(myclient)				"433 " + myclient + " " + myclient  + " :Nickname is already in use"
#define ERR_CMDNEEDMOREPARAMS(myclient, command)		"461 " + myclient + " " + command + "  :Not enough parameters"
#define ERR_UNKNOWNCOMMAND(myclient, command)			"421 " + myclient + " " + command + " :Unknown command"

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result);
