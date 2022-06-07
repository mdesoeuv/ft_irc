#include "../../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

//TO DO: verifier comportement si deja join
void JoinCommand::execute(Client& client, std::string arguments) {
	//check if Channel exist
	std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(arguments);
	if (result.first)
	{
	//si oui:
	//client join le channel
		result.second->addUser(client);
		sendJoinNotif(client, *result.second);
		return ;
	}
	// check if channel_name is not valid
	if (invalidChannelName(arguments))
		return ;
	//TODO : error messages pour les differents cas d'erreur
	Channel	new_channel(arguments);
	Client	client_copy = client;
	client_copy.setChanPrefix("@");
	new_channel.addUser(client_copy);
	new_channel.addOp(client);
	_server->addChannel(new_channel);
	sendJoinNotif(client, new_channel);
}

void JoinCommand::sendJoinNotif(Client& client, Channel channel) {
	channel.broadcastMessage(":" + client.getPrefix() + " JOIN " + channel.getName());
	if (!channel.getTopic().empty())
		client.reply(RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
	else
		client.reply(RPL_NOTOPIC(client.getNickname(), channel.getName()));
	client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getName(), channel.getUserList()));
	client.reply(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));
}

bool JoinCommand::invalidChannelName(const std::string& channel_name) {
	if (channel_name.empty())
		return true;
	if (channel_name.size() > 50)
		return true;
	if (channel_name[0] != '&' && channel_name[0] != '#' && channel_name[0] !=  '+' && channel_name[0] !=  '!')
		return true;
	if (channel_name.find(" ") < channel_name.size() || channel_name.find(",") < channel_name.size() || channel_name.find("^G") < channel_name.size())
		return true;
	return false;
}