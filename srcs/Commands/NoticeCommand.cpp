#include "../../inc/Command.hpp"

NoticeCommand::NoticeCommand(Server *server) : Command(server) {};

NoticeCommand::~NoticeCommand() {};

void NoticeCommand::execute(Client &client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		return;
	}

	std::string target = splited_args[0];
	std::string message;

	for (std::vector<std::string>::iterator it = splited_args.begin() + 1; it != splited_args.end(); it++)
	{
		message.append(*it + " ");
	}

	message = message.at(0) == ':' ? message.substr(1) : message;

	if (target.at(0) == '#') {

		Channel channel;
		try
		{
			channel = _server->getChannel(target.substr(1));
		}
		catch (const std::out_of_range &e)
		{
			return;
		}

		if (channel.getModes().find("n") < channel.getModes().size())
		{ // external msg option == false
			if (!(channel.isUser(client.getNickname())))
			{
				return;
			}
		}
		channel.broadcastMessage(RPL_NOTICE(client.getPrefix(), target, message));
		return;
	}

	Client *dest = _server->getClient(target);
	if (!dest)
	{
		return;
	}
	dest->write(RPL_NOTICE(client.getPrefix(), target, message));
}