#pragma once

#include <vector>
#include <string>
#include "Client.hpp"

class Client;

class Channel
{
	private:

		std::vector<Client>	_user_list;
		std::vector<Client>	_op_list;
		std::string			_name;
		std::string			_topic;
		std::string			_modes;
		
	public:

		Channel(void);
		Channel(const std::string& chan_name, const std::string& chan_topic = "default topic");
		Channel(const Channel& other);
		~Channel(void);

		Channel& operator=(const Channel& rhs);

		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getModes() const;
		void		setName(const std::string new_name);
		void		setTopic(const std::string new_topic);
		void		setModes(const std::string new_modes);
		bool		isUser(const std::string nick) const;
		bool		isOp(const std::string nick) const;
		void		addUser(Client user);
		void		addOp(Client op);
		void		delUser(Client user);
		void		delOp(Client op);
		std::string getUserList() const;
		void		broadcastMessage(std::string message);


};