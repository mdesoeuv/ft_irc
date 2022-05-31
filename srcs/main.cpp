#include "../inc/server.hpp"

int	main(int ac, char** av)
{
	if (ac != 3)
	{
		std::cout << "usage: ft_irc <port> <password>" << std::endl;
		return 1;
	}
	Server server(av[1], av[2]);
	server.start();
	return 0;

}