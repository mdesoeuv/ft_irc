#include "../inc/Client.hpp"

Client::Client( int fd, 
				const std::string& nick, 
				std::string user = "unregistered", 
				std::string pass = "unregistered" ) : 
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

const std::string&	Client::getFullId(const std::string& host) const {
	return (getNickname() + "!" + getUsername() + "@" + host);
}

void				Client::setNickname(const std::string& new_nickname) {
	_nickname = new_nickname;
}

void				Client::setUsername(const std::string& new_username) {
	_username = new_username;
}

void				Client::setPassword(const std::string& new_password) {
	_password = new_password;
}

void				Client::setSocketfd(int new_socketfd) {
	_socketfd = new_socketfd;
}
