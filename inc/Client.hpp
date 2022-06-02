#pragma once

#include <string>

class Client
{
	public:

		Client( int fd,
				const std::string& nick, 
				std::string user = "unregistered", 
				std::string pass = "unregistered" );

		~Client(void);

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getPassword() const;
		int					getSocketfd() const;
		const std::string&	getFullId(const std::string& host) const;

		void				setNickname(const std::string& new_nickname);
		void				setUsername(const std::string& new_username);
		void				setPassword(const std::string& new_password);
		void				setSocketfd(int new_socketfd);

	private:

		Client(void);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		
		int			_socketfd;
		std::string _nickname;
		std::string _username;
		std::string _password;

};