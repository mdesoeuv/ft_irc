#include "../inc/Command.hpp"

NickCommand::NickCommand(Server *server) : Command(server) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments) {
	(void)arguments;
	(void)client;
	//client->setUsername(arguments[1]);
	//client->setRealName(arguments[2]);
	//client->welcome();
}