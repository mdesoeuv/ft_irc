#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

#include "../inc/CommandHandler.hpp"

CommandHandler::CommandHandler(Server *server) : _server(server)
{
	_commands["NICK"] = new NickCommand(_server);
	_commands["USER"] = new UserCommand(_server);
	_commands["QUIT"] = new QuitCommand(_server);
	// _commands["PASS"] = new PassCommand(_server);

	// _commands["PING"] = new PingCommand(_server);
	// _commands["PONG"] = new PongCommand(_server);
	// _commands["JOIN"] = new JoinCommand(_server);
	// _commands["MODE"] = new ModeCommand(_server);
	// _commands["PART"] = new PartCommand(_server);
	// _commands["KICK"] = new KickCommand(_server);

	// _commands["PRIVMSG"] = new PrivMsgCommand(_server);
	// _commands["NOTICE"] = new NoticeCommand(_server);
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
	splitCommand(arguments, message);

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
		
		std::cout <<"Command unknown :" << std::endl;
		std::cout << message << std::endl;
		//client->reply("Command unknown");
	}
}

void	CommandHandler::splitCommand(std::vector<std::string> &arguments, const std::string& message)
{

	size_t				pos = 0;

	pos = message.find(" ");
	if (pos < message.size())
	{
		arguments.push_back(message.substr(0, pos));
		arguments.push_back(message.substr(pos + 1));
	}
	else
		arguments.push_back(message);
}