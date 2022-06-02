#include "../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client *client, std::vector<std::string> arguments) {
	(void)arguments;
	//client->setUsername(arguments[0]);
	//client->setRealName(arguments[1]);
	client->welcome();
}