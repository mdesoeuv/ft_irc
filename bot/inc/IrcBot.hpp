#pragma once


#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/socket.h> // socket()
#include <netinet/in.h> // sockaddr_in
#include <utility> // std::make_pair
#include <unistd.h> // close
#include <arpa/inet.h> /* htons et inet_aton */
#include "utils.hpp"

class IrcBot {
public:
	IrcBot(const std::string &host, const std::string &port, const std::string &password);

	~IrcBot();

	void start();

private:
	int _sock;
	std::string _host;
	std::string _port;
	std::string _password;
	std::string _buffer;

	static void receiver(IrcBot *instance);
	int			newSocket();
	void 		sendMessageToServer(const std::string &message);
	void 		sendPrivMsg(const std::string &source, const std::string &message);
	void 		authenticate(const std::string &nickname);
	void 		onMessageReceived(const std::string &message);
};
