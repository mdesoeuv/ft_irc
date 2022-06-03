#pragma once

class CommandHandler;
class Command;

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Command.hpp"
#include "Server.hpp"
#include "utils.hpp"

class Server;
class CommandHandler
{

private:
	Server *_server;
	std::map<std::string, Command *> _commands;

public:

	typedef std::map<std::string, Command *>::iterator commands_iterator;
	CommandHandler(Server *server);

	~CommandHandler();

	void parsing(Client *client, std::string message);
	void splitCommand(std::vector<std::string> &arguments, const std::string& message);
};
