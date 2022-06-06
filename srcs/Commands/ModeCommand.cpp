#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

// uniquement channel mode pour le moment 
void ModeCommand::execute(Client& client, std::string arguments) {
	std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(arguments);
	{
		if (result.first)
		{
			client.reply(RPL_CHANNELMODEIS(client.getNickname(), result.second->getName(), result.second->getModes(), ""));
			return ;
		}
	}
	//TO DO error handling
}
