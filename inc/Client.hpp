#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h> // socket()

class Client
{
	public:

		Client( int fd,
				const std::string& nick, 
				std::string user = "unregistered", 
				std::string pass = "unregistered" );

		Client(const Client& other);
		
		~Client(void);

		Client&	operator=(const Client& other);

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getPassword() const;
		int					getSocketfd() const;
		std::string 		getPrefix() const;
		std::string			getFullId(const std::string& host) const;
		void 				write(const std::string &message) const;
		void 				welcome();
		void				reply(const std::string &reply);

		void				setNickname(const std::string& new_nickname);
		void				setUsername(const std::string& new_username);
		void				setPassword(const std::string& new_password);
		void				setSocketfd(int new_socketfd);

	private:

		Client(void);
		
		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _password;
		std::string	_hostname;
		std::string _port;

};