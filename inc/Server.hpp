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
#include <arpa/inet.h> /* htons et inet_aton */
#include "CommandHandler.hpp"
#include "utils.hpp"
#include <netdb.h>
#include "Channel.hpp"

#define BUFFER_SIZE 100
#define MAX_CONNECTIONS 100

class Server;
class Client;
class CommandHandler;
class Channel;

class Server
{

	typedef std::vector<pollfd>::iterator pollfds_iterator;
	typedef std::map<int, Client >::iterator clients_iterator;
	typedef std::pair<bool, std::vector<Channel>::iterator> chan_it_pair;

	int							_running;
	int							_sock;
	const std::string			_host;
	const std::string			_port;
	const std::string			_password;
	std::vector<pollfd>			_pollfds;
	std::map<int, Client>		_clients;
	std::vector<Channel>		_channels;
	std::vector<int>			_fdToDelete;
	CommandHandler*				_commandHandler;

public:

	Server(const std::string port, const std::string password);
	~Server();

	void 			start();
	std::string 	getPassword() const { return _password; };
	Client*			getClient(const std::string nickname);
	Channel&		getChannel(const std::string& channel_name);
	void			addClientToDelete(int fd);
	int				newSocket();
	void			onClientConnect();
	void			onClientMessage(int fd);
	void			onClientDisconnect(int fd);
	void			deleteClient(int fd);
	void			addChannel(Channel channel);
	void			removeChannel(std::vector<Channel>::iterator pos);
	void		 	readMessage(int fd);
	void			sendMessage(Client& client);
	chan_it_pair	searchChannel(const std::string channel_name);
	void			allChannelLeave(Client client, std::string broadcast_message);

};

