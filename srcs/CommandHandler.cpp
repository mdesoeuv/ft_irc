#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#include "../inc/CommandHandler.hpp"
#include "../srcs/UserCommand.cpp"

CommandHandler::CommandHandler(Server *server) : _server(server)
{
	_commands["USER"] = new UserCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}

void CommandHandler::parsing(Client *client, const std::string &message)
{

		try
		{
			//parsing split : command_name puis arguments
			Command *command = _commands.at(command_name);
			command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			client->reply("Commande unknown");
		}
}