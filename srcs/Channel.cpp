#include "../inc/Channel.hpp"

Channel::Channel(void) {
}

Channel::Channel(const std::string& chan_name) : 
					_name(chan_name), _topic(""), _modes("+nt"), _user_nb(0) {
}

Channel::Channel(const Channel& other) :	_user_list(other._user_list),
											_op_list(other._op_list),
											_name(other._name),
											_topic(other._topic),
											_modes(other._modes),
											_user_nb(other._user_nb) {
}

Channel::~Channel(void) {
}

Channel&	Channel::operator=(const Channel& rhs) {
	_user_list = rhs._user_list;
	_op_list = rhs._op_list;
	_name = rhs._name;
	_topic = rhs._topic;
	_modes = rhs._modes;
	_user_nb = rhs._user_nb;

	return *this;
}

std::string	Channel::getName() const {
	return _name;
}

std::string	Channel::getTopic() const {
	return _topic;
}

std::string Channel::getModes() const {
	return _modes;
}

size_t		Channel::getUserNb() const {
	return _user_nb;
}

Client&		Channel::getChanClient(const std::string& client_name) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == client_name)
			return *it;
	}
	throw std::out_of_range("client not found");
}

void	Channel::setName(const std::string new_name) {
	_name = new_name;
}

void	Channel::setTopic(const std::string new_topic) {
	_topic = new_topic;
}

void	Channel::setModes(const std::string new_modes) {
	_modes = new_modes;
}

bool	Channel::addMode(char mode) {
	if (isMode(mode))
		return false;
	_modes += mode;
	return true;
}

bool	Channel::removeMode(char mode) {
	if (!isMode(mode))
		return false;
	size_t pos = _modes.find(mode);
	_modes.erase(pos);
	return true;
}

bool	Channel::isUser(const std::string nick) const {
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == nick)
			return true;
	}
	return false;
}


//TODO: rework with modes string and isMode() bool from client
bool	Channel::isOp(const std::string op) const {
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == op)
			return (it->getModes().find("@") < it->getModes().size());
	}
	return false;
}

bool	Channel::isClientMode(const std::string nick, char mode) const {
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		std::cout << it->getNickname() + "is mode " << mode << " ? " << it->isMode(mode) << std::endl;
		std::cout << "client modes string :" + it->getModes() << std::endl;

		if (it->getNickname() == nick)
			return (it->isMode(mode));
	}
	return false;
}

bool	Channel::isInvited(const std::string nickname) const {
	for (std::vector<std::string>::const_iterator it = _user_invited_list.begin(); it != _user_invited_list.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void	Channel::addInvitation(const std::string nickname) {
	_user_invited_list.push_back(nickname);
}

bool	Channel::isMode(char mode) const {
	return (getModes().find(mode) < getModes().size());
}

void	Channel::addUser(Client user) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
			return ;
	}
	_user_list.push_back(user);
	_user_nb++;
}

void	Channel::delUser(Client user) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
		{
			_user_list.erase(it);
			_user_nb--;
			return ;
		}
	}
	// Error message if user is not on channel
	user.reply(ERR_NOTONCHANNEL(user.getNickname(), this->getName()));
}

std::string	Channel::getUserList() const {
	std::string result;
	
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		result += " ";
		result += it->getModes();
		result += it->getNickname();
	}
	return result;
}

void	Channel::broadcastMessage(std::string message) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		it->write(message);
	}
}

void	Channel::broadcastExceptSource(std::string message, const std::string& source_nick) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() != source_nick)
			it->write(message);
	}
}
