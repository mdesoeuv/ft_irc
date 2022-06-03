#include "../inc/Channel.hpp"

Channel::Channel(void) {
}

Channel::Channel(const std::string& chan_name, const std::string& chan_topic) : 
					_name(chan_name), _topic(chan_topic) {
}

Channel::Channel(const Channel& other) :	_user_list(other._user_list),
											_op_list(other._op_list),
											_name(other._name),
											_topic(other._name) {
}

Channel::~Channel(void) {
}

Channel&	Channel::operator=(const Channel& rhs) {
	_user_list = rhs._user_list;
	_op_list = rhs._op_list;
	_name = rhs._name;
	_topic = rhs._topic;

	return *this;
}

std::string	Channel::getName() const {
	return _name;
}

std::string	Channel::getTopic() const {
	return _topic;
}

void	Channel::setName(const std::string new_name) {
	_name = new_name;
}

void	Channel::setTopic(const std::string new_topic) {
	_topic = new_topic;
}

bool	Channel::isUser(const std::string nick) const {
	for (std::vector<Client *>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->_nickname == nick)
			return true;
	}
	return false;
}

bool	Channel::isOp(const std::string op) const {
	for (std::vector<Client *>::iterator it = _op_list.begin(); it != _op_list.end(); ++it)
	{
		if (it->_nickname == op)
			return true;
	}
	return false;
}

void	Channel::addUser(Client* user) {
	_user_list.push_back(user);
}

void	Channel::addOp(Client* op) {
	_op_list.push_back(op);
}


void	Channel::delUser(Client* user) {
	for (std::vector<Client *>::iterator it = _user_list.begin(); it != _user_list.end(); ++it)
	{
		if (it->_nickname == user)
		{
			_user_list.erase(it);
			return ;
		}
	}
}

void	Channel::delOp(Client* op) {
	for (std::vector<Client *>::iterator it = _op_list.begin(); it != _op_list.end(); ++it)
	{
		if (it->_nickname == op)
			_op_list.erase(it);
		return ;
	}
}

