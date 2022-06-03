#include "../../inc/Command.hpp"

UserCommand::UserCommand(Server *server) : Command(server) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client& client, std::string arguments) {
	//checker si user déjà registered ?
	
	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 4) {
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), std::string("USER")));
		return;
	}
	client.setUsername(splited_args[0]);
	client.setRealName(splited_args[3]);
	std::cout <<"Client Username set : " << client.getUsername() << std::endl;
	std::cout <<"Client RealName set : " << client.getRealName() << std::endl; // todo delete : in front of splited_args[3]
	client.welcome();
}