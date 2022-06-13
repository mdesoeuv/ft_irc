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
		std::string			_ban_list;
		size_t				_user_nb;
		//size_t				_user_limit;
		
	public:

		Channel(void);
		Channel(const std::string& chan_name);
		Channel(const Channel& other);
		~Channel(void);

		Channel& operator=(const Channel& rhs);

		std::string	getName() const;
		std::string	getTopic() const;
		std::string	getModes() const;
		size_t		getUserNb() const;
		Client&		getChanClient(const std::string& client_name);
		void		setName(const std::string new_name);
		void		setTopic(const std::string new_topic);
		void		setModes(const std::string new_modes);
		bool		isUser(const std::string nick) const;
		bool		isOp(const std::string nick) const;
		bool		isMode(char mode);
		bool		addMode(char mode);
		bool		removeMode(char mode);
		void		addUser(Client user);
		void		addOp(Client op);
		void		delUser(Client user);
		void		delOp(Client op);
		std::string getUserList() const;
		void		broadcastMessage(std::string message);
		void		broadcastExceptSource(std::string message, const std::string& source_nick);

};