#include "../inc/utils.hpp"

#include <string>
#include <vector>

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result)
{
	std::string message = str;
	size_t		pos = 0;

	while (message.size() != 0)
	{
		pos = message.find(sep);
		if (pos > message.size())
		{
			result.push_back(message);
			break;
		}
		result.push_back(message.substr(0, pos));
		message.erase(0, pos + sep.size());
	}

}

void	sig_quit(int code)
{
	(void)code;
	//TO DO mettre server en variable global
	/*
				for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
			{
				it->second.write(RPL_QUIT(it->second.getPrefix(), "Server is being turned down"));
				allChannelLeave(it->second, RPL_QUIT(it->second.getPrefix(), "Client has been kick beacause Server is being turned down"));
				_fdToDelete.push_back(it->second.getSocketfd());
			}
			close(_sock);
			std::cout << std::endl;
			std::cout << "Server has been turned down. Goodbye !" << std::endl;
			_running = 0;
			*/
}