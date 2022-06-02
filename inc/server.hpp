#pragma once

class Server;
class Client;

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <poll.h>

#define MAX_CONNECTIONS 100

class Server
{

	typedef std::vector<pollfd>::iterator pollfds_iterator;
	typedef std::map<int, Client >::iterator clients_iterator;

	int							_running;
	int							_sock;
	const std::string			_host;
	const std::string			_port;
	const std::string			_password;
	std::vector<pollfd>			_pollfds;
	std::map<int, Client>		_clients;

public:
	Server(const std::string &port, const std::string &password);
	~Server();

	void start();

	std::string getPassword() const { return _password; };
	Client& getClient(const std::string &nickname);

	int newSocket();
	void onClientConnect();
	void onClientMessage(int fd);
	void onClientDisconnect(int fd);
};

