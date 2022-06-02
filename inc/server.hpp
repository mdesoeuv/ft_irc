#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <poll.h>
#include <sys/socket.h> // socket()
#include <fcntl.h> //fcnt parameters
#include <netinet/in.h> // sockaddr_in
#include <utility> // std::make_pair
#include <unistd.h> // close
#include "Client.hpp"

#define MAX_CONNECTIONS 100

class Server;
class Client;

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

	int			newSocket();
	void		onClientConnect();
	void		onClientMessage(int fd);
	void		onClientDisconnect(int fd);
	bool		addClient(int fd);
	void		deleteClient(int fd);
	std::string readMessage(int fd);
};

