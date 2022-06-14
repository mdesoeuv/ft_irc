#include "../../inc/Command.hpp"

KickCommand::KickCommand(Server *server) : Command(server) {}

KickCommand::~KickCommand() {}

void KickCommand::execute(Client& client, std::string arguments) {
	
	// checks if channel exists and get a ref to it
	size_t pos = arguments.find(" ");
	std::string channel_name = arguments.substr(0, pos);

	Channel channel;
	try
	{
		channel = _server->getChannel(channel_name);
	}
	catch(const std::out_of_range& e)
	{
		client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channel_name));
		return ;
	}

	// checks if user has privileges to kick
	Client	user;
	try
	{
		user = channel.getChanClient(client.getNickname());
		if (user.getChanPrefix().find("@") > user.getChanPrefix().size())
		{
			user.reply(ERR_CHANOPRIVSNEEDED(user.getNickname(), channel_name));
			return ;
		}
	}
	catch(const std::out_of_range& e)
	{
		client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel_name));
		return ;
	}


	// parseExecute user_to_kick nickname
	size_t colon_pos = arguments.find(":");
	if (colon_pos >= arguments.size())
	{
		arguments.append(" :Inapropriate behavior");
		colon_pos = arguments.find(":");
	}
	std::string user_to_kick = arguments.substr(pos + 1, colon_pos - 2 - pos);

	// checks if user_to_kick is on channel and kicks
	Client	kicked_user;
	try
	{
		kicked_user = channel.getChanClient(user_to_kick);
		//TODO: anonymise if channel.isMode('a')
		channel.broadcastMessage(":" + user.getPrefix() + " KICK " + arguments);
		channel.delUser(kicked_user);
		if (channel.getUserList(true).empty())
      		_server->removeChannel(_server->searchChannel(channel_name).second);
		std::cout << kicked_user.getNickname() + " kicked !!!" << std::endl;
	}
	catch(const std::out_of_range& e)
	{
		client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), user_to_kick, channel_name));
		return ;
	}

}
