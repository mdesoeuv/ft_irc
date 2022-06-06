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
	//TODO : error messages pour les differents cas d'erreur
	//TODO : avec ses modes par defaut +t +n
	Channel	new_channel(arguments, "");
	new_channel.addUser(client);
	new_channel.addOp(client);
	_server->_channels.push_back(new_channel); // TO DO server.addChannel to set _channels to private
	sendJoinNotif(client, new_channel);
}

void JoinCommand::sendJoinNotif(Client& client, Channel channel) {
	channel.broadcastMessage(":" + client.getPrefix() + " JOIN " + channel.getName());
	if (!channel.getTopic().empty())
		client.reply(RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic())); // TO DO: prefixer ces message avec id server 
	else
		client.reply(RPL_NOTOPIC(client.getNickname(), channel.getName()));
	client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getName(), channel.getUserList()));
	client.reply(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));
}