#pragma once

class Command;

#include <string>
#include "Server.hpp"

class Server;
class Command
{

protected:
	Server *_server;

public:
	explicit Command(Server *server) : _server(server){};
	virtual ~Command(){};

	virtual void execute(Client *client, std::vector<std::string> arguments) = 0;
};

class UserCommand : public Command
{
public:
	UserCommand(Server *server, bool authRequired);
	~UserCommand();

	void execute(Client *client, std::vector<std::string> arguments);
};