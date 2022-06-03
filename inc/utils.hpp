#pragma once

//Error Messages
#define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :Nickname not given"
#define ERR_NICKNAMEALREADYUSED(client)						"433 " + client + " " + client  + " :Nickname is already in use"
#define ERR_CMDNEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters"
