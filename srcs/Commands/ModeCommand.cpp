#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

// uniquement channel mode pour le moment 
void ModeCommand::execute(Client& client, std::string arguments) {
	for(std::vector<Channel>::iterator it = _server->_channels.begin(); it !=_server->_channels.end(); ++it)
	{
		if (it->getName() == arguments)
		{
			client.reply(RPL_CHANNELMODEIS(client.getNickname(), it->getName(), it->getModes(), ""));
			return ;
		}
	}
	//TO DO error handling
}
