#pragma once

//Error Messages
#define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(client)						"433 " + client + " " + client  + " :Nickname is already in use"
#define ERR_CMDNEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters"


// replies 
#define RPL_QUIT(client, message)					":" + client + " QUIT :Quit: " + message
#define RPL_WELCOME(client)						"001 " + client + " :Welcome " + client + " to the ft_irc network"
