#include "IrcBot.hpp"

IrcBot::IrcBot(const std::string &host, const std::string &port, const std::string &password) :
		_host(host), _port(port), _password(password) {
	_sock = newSocket();
}

IrcBot::~IrcBot() {
}