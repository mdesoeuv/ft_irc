#include "../../inc/Command.hpp"

NickCommand::NickCommand(Server *server) : Command(server) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> arguments) {
	if (arguments.empty() || arguments[1].empty()) {
		client->reply(ERR_NONICKNAMEGIVEN(client->getNickname()));
		return;
	}

	if (_server->getClient(arguments[1])) {
		client->reply(ERR_NICKNAMEALREADYUSED(client->getNickname()));
		return;
	}
	std::cout <<"Client Nickname set : " << arguments[1] << std::endl;
	client->setNickname(arguments[1]);
}