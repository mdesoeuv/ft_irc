#include "../inc/Client.hpp"

Client::Client( int fd, 
				const std::string& nick, 
				std::string user, 
				std::string pass) : 
				_socketfd(fd), _nickname(nick), _username(user), _password(pass) {
}

Client::Client(const Client& other) :	_socketfd(other.getSocketfd()), 
										_nickname(other.getNickname()),
										_username(other.getUsername()),
										_password(other.getPassword()) {
}

Client::~Client() {
}

Client&	Client::operator=(const Client& rhs) {
	_socketfd = rhs.getSocketfd();
	_nickname = rhs.getNickname();
	_username = rhs.getUsername();
	_password = rhs.getPassword();

	return *this ;
}

const std::string&	Client::getNickname() const {
	return _nickname ;
}

const std::string&	Client::getUsername() const {
	return _username ;
}

const std::string&	Client::getPassword() const {
	return _password ;
}

std::string	Client::getFullId(const std::string& host) const {
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
