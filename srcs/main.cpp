#include "../inc/server.hpp"

int main(int ac, char **av)
{
	try
	{
		if (argc != 3)
		{
			throw std::runtime_error("Usage: ./ircserv <port> <password>");
		}
		Server server(argv[1], argv[2]);
		server.start();
		return 0;
	}
	catch (const std::exception &except)
	{
		std::cerr << except.what() << std::endl;
		return 1;
	}
}