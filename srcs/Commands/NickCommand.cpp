#include "../../inc/Command.hpp"

NickCommand::NickCommand(Server *server) : Command(server) {}

//TODO: broadcast nick changes in accordance to anonymous channel flag
NickCommand::~NickCommand() {}

void NickCommand::execute(Client& client, std::string arguments) {
	if (arguments.empty()) {
		client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()));
		return;
	}

	if (_server->getClient(arguments)) {
		client.reply(ERR_NICKNAMEALREADYUSED(client.getNickname()));
		return;
	}
	client.setNickname(arguments);
	std::cout << "Client Nickname set : " << client.getNickname() << std::endl;
	client.welcome();
}