#pragma once


#include <string>
#include "Server.hpp"
#include "Channel.hpp"
#include "utils.hpp"

class Server;
class Channel;

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

	void sendJoinNotif(Client& client, Channel channel);

	void execute(Client& client, std::string arguments);
};

class PartCommand : public Command
{
public:
	PartCommand(Server *server);
	~PartCommand();

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

class PingCommand : public Command
{
public:
	PingCommand(Server *server);
	~PingCommand();

	void execute(Client& client, std::string arguments);
};

class PassCommand : public Command
{
public:
	PassCommand(Server *server);
	~PassCommand();

	void execute(Client& client, std::string arguments);
};

class PongCommand : public Command
{
public:
	PongCommand(Server *server);
	~PongCommand();

	void execute(Client& client, std::string arguments);
};