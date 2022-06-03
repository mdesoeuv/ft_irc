#pragma once

/* replies Message */

#define RPL_WELCOME(source)							"001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_QUIT(client, message)					":" + client + " QUIT :Quit: " + message

/* Error Messages */

#define ERR_NONICKNAMEGIVEN(client)					"431 " + client + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(client)				"433 " + client + " " + client  + " :Nickname is already in use"
#define ERR_CMDNEEDMOREPARAMS(client, command)		"461 " + client + " " + command + " :Not enough parameters"