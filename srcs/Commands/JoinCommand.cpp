#include "../../inc/Command.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server) {}

JoinCommand::~JoinCommand() {}

// TODO: JOIN 0 = leave all channels
void JoinCommand::execute(Client &client, std::string arguments)
{

	// check if Channel exist
	std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(arguments);
	if (result.first)
	{
		// checks if client is already on channel
		if (result.second->isUser(client.getNickname()))
		{
			client.reply(ERR_USERONCHANNEL(client.getNickname(), client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isMode('i') && !result.second->isInvited(client.getNickname()))
		{
			client.reply(ERR_CLIENTNOTINVITED(client.getNickname(), result.second->getName()));
			return;
		}

		if (result.second->isMode('b') && !result.second->isBanned(client.getNickname()))
		{
			client.reply(ERR_BANNEDFROMCHAN(client.getNickname(), result.second->getName()));
			return;
		}

		// TODO: verifier la limite d'utilisateurs du channel

		// client join le channel
		result.second->addUser(client);
		sendJoinNotif(client, *result.second);
		return;
	}
	// check if channel_name is not valid
	if (invalidChannelName(arguments))
	{
		client.reply(ERR_INVALIDCHANNELNAME(client.getNickname(), arguments));
		return;
	}
	Channel new_channel(arguments);
	Client client_copy = client;
	client_copy.setPtr(&client);
	client_copy.setChanPrefix("@");
	client_copy.addUserMode('@');
	new_channel.addUser(client_copy);
	// new_channel.addOp(client);
	_server->addChannel(new_channel);
	sendJoinNotif(client, new_channel);
}

void JoinCommand::sendJoinNotif(Client &client, Channel channel)
{
	channel.broadcastMessage(":" + client.getPrefix() + " JOIN " + channel.getName());
	if (!channel.getTopic().empty())
		client.reply(RPL_TOPIC(client.getNickname(), channel.getName(), channel.getTopic()));
	else
		client.reply(RPL_NOTOPIC(client.getNickname(), channel.getName()));
	client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getName(), channel.getUserList()));
	client.reply(RPL_ENDOFNAMES(client.getNickname(), channel.getName()));
}

bool JoinCommand::invalidChannelName(const std::string &channel_name)
{
	if (channel_name.empty())
		return true;
	if (channel_name.size() > 50 || channel_name.size() < 2)
		return true;

	/*
	** restricted channel name creation to #channel only
	** normal condition would be :
	** if (channel_name[0] != '#' && channel_name[0] !=  '+' && channel_name[0] !=  '!' && channel_name[0] != '&')
	*/

	if (channel_name[0] != '#')
		return true;
	if (channel_name.find(" ") < channel_name.size() || channel_name.find(",") < channel_name.size() || channel_name.find("^G") < channel_name.size())
		return true;
	return false;
}