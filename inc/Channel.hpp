#pragma once

#include <vector>
#include <string>
#include "Client.hpp"

class Channel
{
	private:

		std::vector<std::string>	_user_list;
		std::vector<std::string>	_op_list;
		std::string					_name;
		std::string					_topic;
		
	public:

		Channel(void);
		Channel(const std::string& chan_name, const std::string& chan_topic = "default topic");
		Channel(const Channel& other);
		~Channel(void);

		Channel& operator=(const Channel& rhs);

		std::string	getName() const;
		std::string	getTopic() const;
		void		setName(const std::string new_name);
		void		setTopic(const std::string new_topic);
		bool		isUser(const std::string nick) const;
		bool		isOp(const std::string nick) const;
		void		addUser(const std::string user);
		void		addOp(const std::string op);
		void		delUser(const std::string user);
		void		delOp(const std::string op);


};