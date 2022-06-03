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

		void	getName();
		void	getTopic();
		void	setName(const std::string new_name);
		void	setTopic(const std::string new_topic);
		bool	is_user(const std::string nick) const;
		bool	is_op(const std::string nick) const;



};