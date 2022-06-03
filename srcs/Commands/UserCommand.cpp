#include "../../inc/Command.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) {
	(void)arguments;
	//client->setUsername(arguments[0]);
	//client->setRealName(arguments[1]);
	client->welcome();
}