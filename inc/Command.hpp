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

	void sendJoinNotif(Client& client, Channel channel, bool anonynous);
	bool invalidChannelName(const std::string& channel_name);

	void execute(Client& client, std::string arguments);
};

class ModeCommand : public Command
{
public:
	ModeCommand(Server *server);
	~ModeCommand();

	void execute(Client& client, std::string arguments);

	bool applyMode(Channel &channel, Client &client, bool active, char c, std::string arg);
	void mode_channel(Channel& channel, Client& client, std::vector<std::string> splited_args);
	void mode_client(Client* client,  std::vector<std::string> splited_args);

};

class PartCommand : public Command
{
public:
	PartCommand(Server *server);
	~PartCommand();

	void execute(Client& client, std::string arguments);
};

class TopicCommand : public Command
{
public:
	TopicCommand(Server *server);
	~TopicCommand();

	void execute(Client& client, std::string arguments);
};

class KickCommand : public Command
{
public:
	KickCommand(Server *server);
	~KickCommand();

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

class NamesCommand : public Command
{
public:
	NamesCommand(Server *server);
	~NamesCommand();

	void execute(Client& client, std::string arguments);
};

class ListCommand : public Command
{
public:
	ListCommand(Server *server);
	~ListCommand();

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

class PrivMsgCommand : public Command
{
public:
	PrivMsgCommand(Server *server);
	~PrivMsgCommand();

	void execute(Client& client, std::string arguments);
};

class NoticeCommand : public Command
{
public:
	NoticeCommand(Server *server);
	~NoticeCommand();

	void execute(Client& client, std::string arguments);
};

class InviteCommand : public Command
{
public:
	InviteCommand(Server *server);
	~InviteCommand();

	void execute(Client& client, std::string arguments);
};