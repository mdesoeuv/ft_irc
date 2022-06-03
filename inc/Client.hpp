#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h> // socket()
#include "utils.hpp"

class Client
{
	public:

		Client(int fd, const std::string hostname, int port);

		Client(const Client& other);
		
		~Client(void);

		Client&	operator=(const Client& other);

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getRealName() const;
		const std::string&	getPassword() const;
		int					getSocketfd() const;
		int					getPort() const;
		std::string 		getPrefix() const;
		std::string			getFullId(const std::string& host) const;
		void 				write(const std::string &message) const;
		void 				welcome();
		void				reply(const std::string &reply);

		void				setNickname(const std::string& new_nickname);
		void				setRealName(const std::string& new_nickname);
		void				setUsername(const std::string& new_username);
		void				setPassword(const std::string& new_password);
		void				setSocketfd(int new_socketfd);
		void				setPort(int new_port);
		// void				addSendQueue(const std::string msg);

	private:

		Client(void);
		
		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _realName;
		std::string _password;
		std::string	_hostname;
		int _port;
		// std::string	_send_queue;

};