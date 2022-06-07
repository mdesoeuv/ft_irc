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

void	Channel::setName(const std::string new_name) {
	_name = new_name;
}

void	Channel::setTopic(const std::string new_topic) {
	_topic = new_topic;
}

void	Channel::setModes(const std::string new_modes) {
	_modes = new_modes;
}

bool	Channel::isUser(const std::string nick) const {
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == nick)
			return true;
	}
	return false;
}

bool	Channel::isOp(const std::string op) const {
	for (std::vector<Client>::const_iterator it = _op_list.begin(); it != _op_list.end(); ++it)
	{
		if (it->getNickname() == op)
			return true;
	}
	return false;
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

void	Channel::addOp(Client op) {
	for (std::vector<Client>::iterator it = _op_list.begin(); it != _op_list.end(); ++it)
	{
		if (it->getNickname() == op.getNickname())
			return ;
	}
	_op_list.push_back(op);
}


void	Channel::delUser(Client user, const std::string message) {
	for (std::vector<Client>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
		{
    		broadcastMessage(":" + user.getPrefix() + " PART " + message);
			_user_list.erase(it);
			_user_nb--;
			return ;
		}
	}
	// Error message if user is not on channel
	user.reply(ERR_NOTONCHANNEL(user.getNickname(), this->getName()));
}

void	Channel::delOp(Client op) {
	for (std::vector<Client>::iterator it = _op_list.begin(); it != _op_list.end(); ++it)
	{
		if (it->getNickname() == op.getNickname())
			_op_list.erase(it);
		return ;
	}
}

std::string	Channel::getUserList() const {
	std::string result;
	
	for (std::vector<Client>::const_iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		result += " ";
		result += it->getChanPrefix();
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
