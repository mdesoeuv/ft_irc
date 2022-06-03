#include "../../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(Client& client, std::string arguments) {
	//check de l'existance du Channel
	for(std::vector<Channel>::iterator it = _server->_channels.begin(); it !=_server->_channels.end(); ++it)
	{
	//si oui:
	//client join le channel
		if (it->getName() == arguments)
		{
			it->addUser(client);
			sendJoinNotif(client, *it);
			return ;
		}
	}

	//TODO : avec ses modes par defaut +t +n
	Channel	new_channel(arguments);
	new_channel.addUser(client);
	new_channel.addOp(client);
	sendJoinNotif(client, new_channel);
	_server->_channels.push_back(new_channel); // TO DO server.addChannel

}

void JoinCommand::sendJoinNotif(Client& client, Channel channel) {
	channel.broadcastMessage("JOIN " + channel.getName());
	channel.broadcastMessage(RPL_NAMEREPLY(client.getNickname(), channel.getName(), channel.getUserList()));
	channel.broadcastMessage(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));	
}