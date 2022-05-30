#include "../inc/ft_irc.hpp"



int	main(int ac, char** av)
{
	if (ac != 3)
	{
		std::cout << "usage: ft_irc <port> <password>" << std::endl;
		return 1;
	}
	(void)av;
	return 0;

}