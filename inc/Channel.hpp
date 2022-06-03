#pragma once

#include <vector>
#include <string>
#include "Client.hpp"

class Channel
{
	private:

		std::vector<Client *>	_user_list;
		std::vector<Client *>	_op_list;
		std::string				_name;
		std::string				_topic;
		
	public:

		Channel(void);
		Channel(const std::string& chan_name, const std::string& chan_topic = "default topic");
		Channel(const Channel& other);
		~Channel(void);

		Channel& operator=(const Channel& rhs);

		void		getName() const;
		void		getTopic() const;
		std::string	setName(const std::string new_name);
		std::string	setTopic(const std::string new_topic);
		bool		isUser(const std::string nick) const;
		bool		isOp(const std::string nick) const;
		void		addUser(Client* user);
		void		addOp(Client* op);
		void		delUser(Client* user);
		void		delOp(Client* op);


};