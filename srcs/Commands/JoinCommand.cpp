#include "../../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client& client, std::string arguments) {
	(void)arguments;
	(void)client;
	//client->setUsername(arguments[1]);
	//client->setRealName(arguments[2]);
}