#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#include "../inc/CommandHandler.hpp"

CommandHandler::CommandHandler(Server *server) : _server(server)
{
	_commands["USER"] = new UserCommand(_server);
	_commands["JOIN"] = new JoinCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}

void	CommandHandler::parsing(Client *client, const std::string &message)
{
	// split de message: arguments
		std::vector<std::string>	arguments;
		split(arguments, message);

		//display du split pour debug parsing
		for (std::vector<std::string>::iterator it = arguments.begin(); it != arguments.end(); ++it)
			std::cout << *it << std::endl;
		try
		{
			Command *command = _commands.at(arguments[0]);
			command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			client->reply("Command unknown");
		}
		// free arguments
}

void	CommandHandler::split(std::vector<std::string> &arguments, const std::string& message) {

	commands_iterator	iter = _commands.begin();
	size_t				pos = 0;

	while (iter != _commands.end())
	{
		pos = message.find(iter->first);
		if (pos < message.size())
		{
			arguments.push_back(iter->first);
			arguments.push_back(message.substr(pos + iter->first.size()));
			break ;
		}
		++iter;
	}
	arguments.push_back("");

}