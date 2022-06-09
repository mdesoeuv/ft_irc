#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h> // socket()
#include "utils.hpp"
#include <ctime>

class Client
{
	public:
	
		Client(void);

		Client(int fd, const std::string hostname, int port, const std::string chan_prefix);

		Client(const Client& other);
		
		~Client(void);

		Client&	operator=(const Client& other);

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getRealName() const;
		const std::string&	getPassword() const;
		const std::string&	getHostname() const;
		const std::string&	getChanPrefix() const;
		bool 				isAuthentified() const;
		bool 				isRegistered() const;
		int					getSocketfd() const;
		int					getPort() const;
		std::string 		getPrefix() const;
		std::string&		getMessageBuffer();
		std::string&		getSendQueue();
		void 				write(const std::string &message) const;
		void 				welcome();
		void				reply(const std::string &reply);
		void				setIsAuthentified(bool is_Authentified);
		void				setIsRegistered(bool is_Registered);
		void				setNickname(const std::string& new_nickname);
		void				setRealName(const std::string& new_nickname);
		void				setUsername(const std::string& new_username);
		void				setPassword(const std::string& new_password);
		void				setChanPrefix(const std::string new_prefix);
		void				setPtr(Client* client);
		std::string			extractMessage();
		time_t				getLastPingTime();
		void				setLastPingTime(time_t new_lastPingTime);
		void				addSendQueue(const std::string message);

	private:

		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _realName;
		std::string _password;
		std::string	_hostname;
		bool 		_isAuthentified;
		bool 		_isRegistered;
		int 		_port;
		std::string	_chanPrefix;
		std::string _messageBuffer;
		time_t		_lastPingTime;
		std::string	_sendQueue;
		Client*		_clientOnServer;

};