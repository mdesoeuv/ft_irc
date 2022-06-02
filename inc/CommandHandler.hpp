#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#define MAX_CONNECTIONS 100

class Server;
class Client;
class CommandHandler;

class CommandHandler
{

public:
	CommandHandler();
	~CommandHandler();

	void		writeClientMsg(Client client, std::string msg);
	//void		join(Client client);
	//void		quit(Client client);
	//commands list
};

