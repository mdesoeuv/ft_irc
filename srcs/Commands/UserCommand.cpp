#include "../../inc/Command.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client& client, std::string arguments) {
	
	if (client.isRegistered() || !client.isAuthentified()) {
		client.reply(ERR_ALREADYREGISTERED(client.getNickname()));
		return;
	}

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 4) {
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), std::string("USER")));
		return;
	}
	client.setUsername(splited_args[0]);
	client.setRealName(splited_args[3].substr(splited_args[3][0] == ':' ? 1 : 0));
	std::cout <<"Client Username set : " << client.getUsername() << std::endl;
	std::cout <<"Client RealName set : " << client.getRealName() << std::endl; 
}