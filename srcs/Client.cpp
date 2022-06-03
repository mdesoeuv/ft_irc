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

std::string Client::getPrefix() const {
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

void Client::write(const std::string &message) const {

	std::string buffer = message + "\r\n";
	if (send(_socketfd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

void Client::reply(const std::string &reply) {
	write(":" + getPrefix() + " " + reply);
}

void Client::welcome() {
	reply(RPL_WELCOME(_nickname));
	std::cout << "welcome messsage sent to "  << _nickname << std::endl;
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

int					Client::getSocketfd() const {
	return _socketfd ;
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
