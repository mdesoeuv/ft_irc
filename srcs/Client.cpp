#include "../inc/Client.hpp"

Client::Client() {

}

Client::Client(int fd, const std::string hostname, int port, const std::string chan_prefix)
		: _socketfd(fd), _hostname(hostname), _isAuthentified(false), _isRegistered(false), _port(port), _chanPrefix(chan_prefix), _clientOnServer(this) {
	
	_lastPingTime = time(NULL);
}

Client::Client(const Client& other) :	_socketfd(other.getSocketfd()), 
										_nickname(other.getNickname()),
										_username(other.getUsername()),
										_realName(other.getRealName()),
										_password(other.getPassword()),
										_hostname(other._hostname),
										_isAuthentified(other.isAuthentified()),
										_isRegistered(other.isRegistered()),
										_port(other.getPort()),
										_chanPrefix(other.getChanPrefix()),
										_modes(other.getModes()),
										_clientOnServer(other._clientOnServer) {

	_lastPingTime = time(NULL);
	
}

Client::~Client() {
}

Client&	Client::operator=(const Client& rhs) {
	_socketfd = rhs.getSocketfd();
	_nickname = rhs.getNickname();
	_username = rhs.getUsername();
	_realName = rhs.getRealName();
	_password = rhs.getPassword();
	_isAuthentified = rhs.isAuthentified();
	_isRegistered = rhs.isRegistered();
	_hostname = rhs._hostname ;
	_port = rhs.getPort();
	_chanPrefix = rhs.getChanPrefix();
	_modes = rhs.getModes();
	_clientOnServer = rhs._clientOnServer;
	_lastPingTime = rhs._lastPingTime;

	return *this ;
}

std::string Client::getPrefix() const {
	return _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
}

void Client::write(const std::string &message) const {
	_clientOnServer->addSendQueue(message);
}

void Client::reply(const std::string &reply) {
	addSendQueue(_serverPrefix + reply);
}

void Client::welcome() {
	std::cout << "_isAuthentified"  << _isAuthentified << std::endl;
	std::cout << "_username"  << _username << std::endl;
	std::cout << "_realName"  << _realName << std::endl;
	std::cout << "_nickname"  << _nickname << std::endl;
	if (_isAuthentified == false || _username.empty() || _realName.empty() || _nickname.empty())
		return;
	setIsRegistered(true);
	reply(RPL_WELCOME(_nickname));
	std::cout << "welcome messsage sent to "  << _nickname << std::endl;
}

bool	Client::isAuthentified() const {
	return _isAuthentified;
}

bool	Client::isRegistered() const {
	return _isRegistered;
}

const std::string&	Client::getNickname() const {
	return _nickname;
}

const std::string&	Client::getUsername() const {
	return _username;
}

const std::string&	Client::getRealName() const {
	return _realName;
}

const std::string&	Client::getPassword() const {
	return _password;
}

int					Client::getSocketfd() const {
	return _socketfd;
}

int					Client::getPort() const {
	return _port;
}

const std::string&	Client::getHostname() const {
	return _hostname;
}

const std::string&	Client::getChanPrefix() const {
	return _chanPrefix;
}

const std::string&	Client::getModes() const {
	return _modes;
}

time_t		Client::getLastPingTime() {
	return _lastPingTime;
}

std::string&		Client::getMessageBuffer() {
	return _messageBuffer;
}

std::string&		Client::getSendQueue() {
	return _sendQueue;
}


void				Client::setNickname(const std::string& new_nickname) {
	_nickname = new_nickname;
}

void				Client::setUsername(const std::string& new_username) {
	_username = new_username;
}

void				Client::setRealName(const std::string& new_realName) {
	_realName = new_realName;
}

void				Client::setPassword(const std::string& new_password) {
	_password = new_password;
}

void				Client::setIsAuthentified(bool is_Authentified) {
	_isAuthentified = is_Authentified;
}

void				Client::setIsRegistered(bool is_Registered) {
	_isRegistered = is_Registered;
}

void				Client::setChanPrefix(const std::string new_prefix) {
	_chanPrefix += new_prefix;
}

void				Client::setLastPingTime(time_t new_lastPingTime) {
	_lastPingTime = new_lastPingTime;
}

void				Client::setPtr(Client* clientPtr) {
	_clientOnServer = clientPtr;
}

std::string			Client::extractMessage() {

	std::string message; 

	size_t pos = _messageBuffer.find("\r\n");
	message = _messageBuffer.substr(0, pos);
	_messageBuffer.erase(0, pos + 2);

	std::cout << "extracted message :" + message << std::endl;
	return message;
}

void				Client::addSendQueue(const std::string message) {
	_sendQueue += message;
	_sendQueue += "\r\n";
}

void				Client::addUserMode(char mode) {
	_modes += mode;
	std::cout << "user mode string :" + _modes << std::endl;
}

void				Client::removeUserMode(char mode) {
	size_t pos = _modes.find(mode);
	_modes.erase(pos);
	std::cout << "user mode string :" + _modes << std::endl;
}