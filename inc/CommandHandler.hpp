#pragma once

class CommandHandler;

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Command.hpp"
#include "Server.hpp"

class Server;
class CommandHandler
{

private:
	Server *_server;
	std::map<std::string, Command *> _commands;

public:
	CommandHandler(Server *server);

	~CommandHandler();

	void parsing(Client *client, const std::string &message);
};
