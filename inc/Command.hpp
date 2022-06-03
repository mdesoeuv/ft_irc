#pragma once


#include <string>
#include "Server.hpp"
#include "utils.hpp"

class Server;

class Command
{

protected:
	Server *_server;

public:
	explicit Command(Server *server) : _server(server){};
	virtual ~Command(){};

	virtual void execute(Client& client, std::string arguments) = 0;
};

class UserCommand : public Command
{
public:
	UserCommand(Server *server);
	~UserCommand();

	void execute(Client& client, std::string arguments);
};

class JoinCommand : public Command
{
public:
	JoinCommand(Server *server);
	~JoinCommand();

	void execute(Client& client, std::string arguments);
};

class NickCommand : public Command
{
public:
	NickCommand(Server *server);
	~NickCommand();

	void execute(Client& client, std::string arguments);
};

class QuitCommand : public Command
{
public:
	QuitCommand(Server *server);
	~QuitCommand();

	void execute(Client& client, std::string arguments);
};
