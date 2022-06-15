#include "../../inc/Command.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client& client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 4) {
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), std::string("USER")));
		return;
	}
	
	if (client.isRegistered() || _server->getClient(splited_args[0])) {
		client.reply(ERR_ALREADYREGISTERED(client.getNickname()));
		return;
	}

	if (!client.isAuthentified()) {
		client.reply("ERROR : Please connect to server with password.");
		return;
	}
	
	client.setUsername(splited_args[0]);
	client.setRealName(splited_args[3].substr(splited_args[3][0] == ':' ? 1 : 0));
	std::cout <<"Client Username set : " << client.getUsername() << std::endl;
	std::cout <<"Client RealName set : " << client.getRealName() << std::endl; 
	client.welcome();
}