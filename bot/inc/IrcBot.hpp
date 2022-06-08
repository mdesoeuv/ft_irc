#pragma once


#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
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

};
