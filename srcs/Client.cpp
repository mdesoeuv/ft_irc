#include "../inc/Client.hpp"

Client::Client( int fd, 
				const std::string& nick, 
				const std::string& user = "unregistered", 
				std::string& pass = "unregistered" ) : 
				_socketfd(fd), _nickname(nick), _username(user), _password(pass) {
}

Client::Client(const Client& other) :	_socketfd(fd), 
										_nickname(other.getNickname()),
										_username(other.getUsername()),
										_password(other.getPassword()) {
}

Client::~Client() {
}

Client&	Client::operator=(const Client& rhs) {
	_socketfd = other.getSocketfd();
	_nickname = other.getNickname();
	_username = other.getUsername();
	_password = other.getPassword();
}

const std::string&	Client::getNickname() {
	return _nickname ;
}

const std::string&	Client::getUsername() {
	return _username ;
}

const std::string&	Client::getPassword() {
	return _password ;
}


