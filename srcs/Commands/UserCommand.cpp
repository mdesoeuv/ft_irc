#include "../../inc/Command.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> arguments) {
	//checker si déjà registered ?
	std::cout <<"arguments.size() : " << std::endl;

	if (arguments.size() < 4) {
		client->reply(ERR_CMDNEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	client->setUsername(arguments[1]);
	client->setRealName(arguments[4]);
	std::cout <<"Client Username set : " << arguments[1] << std::endl;
	std::cout <<"Client RealName set : " << arguments[4] << std::endl;
	client->welcome();
}