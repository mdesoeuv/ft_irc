#pragma once

#include <string>

class Client
{
	public:

		Client( int fd
				const std::string& nick, 
				const std::string& user = "unregistered", 
				const std::string& pass = "unregistered" );

		~Client(void);

		const std::string&	getNickname();
		const std::string&	getUsername();
		const std::string&	getPassword();
		int					getSocketfd();

	private:

		Client(void);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		
		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _password;

};