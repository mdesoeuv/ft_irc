#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client &client, std::string arguments)
{

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	// checks if a target and a parameter are present
	if (splited_args.size() < 1 || splited_args[0].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
		return;
	}

	std::string target = splited_args[0];

	std::cout << "target of mode command:" + target + "//" << std::endl;
	// check if target is a channel then if client is op and then execute operation
	if (target[0] == '#')
	{
		Channel* channel = _server->getChannel(target);
		if (!channel)
		{
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target));
			return;
		}
		// processing MODE #channel command without parameters
		if (splited_args.size() == 1)
		{
			client.reply(RPL_CHANNELMODEIS(client.getNickname(), channel->getName(), channel->getModes(), ""));
			return;
		}
		// verify if client is op to process other types of MODE cmd
		if (!channel->isOp(client.getNickname()))
		{
			client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), target));
			return;
		}
		if (!channel->isUser(client.getNickname()))
		{
			client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel->getName()));
			return;
		}
		mode_channel(channel, client, splited_args);
	}
	else
	{
		// checks if user exists and execute operation
		Client *target_client = _server->getClient(target);
		if (target_client == nullptr)
			client.reply(ERR_NOSUCHNICK(client.getNickname(), target));
		else if (target_client->getNickname() != client.getNickname())
			client.reply(ERR_USERSDONTMATCH(client.getNickname()));
		else
			mode_client(target_client, splited_args);
	}
}

bool ModeCommand::applyMode(Channel* channel, Client &client, bool active, char c, std::string arg)
{
	if (active)
	{
		if (!channel->addMode(c))
			return false;
	}
	else
	{
		if (!channel->removeMode(c))
			return false;
	}
	channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? std::string(1, '+') + c : std::string(1, '+') + c), (active && !arg.empty() ? arg : "")));
	return true;
}

void ModeCommand::mode_channel(Channel* channel, Client &client, std::vector<std::string> splited_args)
{

	int i = 0;
	int p = 2;
	char c;

	while ((c = splited_args[1][i]))
	{

		char prevC = i > 0 ? splited_args[1][i - 1] : '\0';
		bool active = prevC == '+';

		switch (c)
		{
		case 'a':
		{
			applyMode(channel, client, active, 'a', "");
			break;
		}
		case 'b':
		{
			mode_ban(channel, client, active, splited_args);
			break;
		}

		case 'e':
		{
			mode_exception(channel, client, active, splited_args);
			break;
		}

		case 'I':
		{
			mode_invite(channel, client, active, splited_args);
			break;
		}

		case 'm':
		{
			applyMode(channel, client, active, 'm', "");
			break;
		}

		case 'q':
		{
			applyMode(channel, client, active, 'q', "");
			break;
		}

		case 'k':
		{
			if (splited_args[2].empty())
			{
				client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "CHANNEL PASSWORD"));
				break;
			}
			applyMode(channel, client, active, 'k', splited_args[p]);
			p += active ? 1 : 0;
			break;
		}

		case 'n':
		{
			applyMode(channel, client, active, 'n', "");
			break;
		}

		case 'i':
		{
			applyMode(channel, client, active, 'i', "");
			break;
		}

		case 'o':
		{
			mode_operator(channel, client, active, splited_args);
			break;
		}

		case 'l':
		{
			channel->setChannelLimit(active ? std::stol(splited_args[p]) : 0);
			channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+l" : "-l"), (active ? splited_args[p] : "")));
			p += active ? 1 : 0;
			break;
		}

		case 'p':
		{
			applyMode(channel, client, active, 'p', "");
			break;
		}

		case 's':
		{
			if (applyMode(channel, client, active, 's', ""))
			{
				if (active)
					channel->setSymbol("@");
				else
					channel->setSymbol("=");
			}
			break;
		}

		case 't':
		{
			applyMode(channel, client, active, 't', "");
			break;
		}

		case 'v':
		{
			mode_voice(channel, client, active, splited_args);
			p += active ? 1 : 0;
			break;
		}

		default:
			break;
		}

		i++;
	}
}

void	ModeCommand::mode_ban(Channel* channel, Client& client, bool active, std::vector<std::string> splited_args)
{
	if (splited_args[2].empty())
			{
				client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "BAN"));
				return;
			}
			if (active)
			{
						std::cout << splited_args[2] + " to ban !" << std::endl;

				if (channel->isBanned(splited_args[2]))
				{
					client.reply(ERR_ALREADYBANNED(client.getNickname(), splited_args[2], channel->getName()));
					return;
				}
				channel->addBan(splited_args[2]);
				std::cout << channel->isBanned(splited_args[2]) << std::endl;
				std::cout << " is banned ?" << std::endl;

				channel->broadcastMessage(RPL_BANNED(client.getNickname(), splited_args[2], channel->getName()));
			}
			else
			{
				if (!channel->isBanned(splited_args[2]))
				{
					client.reply(ERR_ALREADYUNBANNED(client.getNickname(), splited_args[2], channel->getName()));
					return;
				}
				channel->removeBan(splited_args[2]);
				channel->broadcastMessage(RPL_UNBANNED(client.getNickname(), splited_args[0], channel->getName()));
			}
			channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+b" : "-b"), splited_args[2]));
}

void	ModeCommand::mode_voice(Channel* channel, Client& client, bool active, std::vector<std::string> splited_args)
{
	// voice mode on channel: '+' before nick

	if (splited_args.size() < 3)
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
		return;
	}
	if (!channel->isUser(splited_args[2]))
	{
		client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), splited_args[2], channel->getName()));
		return;
	}
	try
	{
		Client &target_client = channel->getChanClient(splited_args[2]);
		if (active)
		{
			if (target_client.isMode('+'))
				return;
			target_client.addUserMode('+');
			std::cout << "added voice mode to " + target_client.getNickname() + " for channel " + channel->getName() << std::endl;
		}
		else
		{
			target_client.removeUserMode('+');
			std::cout << "removed voice mode to " + target_client.getNickname() + " for channel " + channel->getName() << std::endl;
		}
		std::cout << "is voice ok ? " << channel->isClientMode(target_client.getNickname(), '+') << std::endl;

		channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+v" : "-v"), target_client.getNickname()));
	}
	catch (std::out_of_range &e)
	{
		std::cout << "Client not found" << std::endl;
		return;
	}
}

void	ModeCommand::mode_operator(Channel* channel, Client& client, bool active, std::vector<std::string> splited_args)
{
	// operator mode: @ before nickname
	std::cout << "active :" << active << std::endl;
	if (splited_args.size() != 3)
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
		return;
	}
	if (!channel->isUser(splited_args[2]))
	{
		client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), splited_args[2], channel->getName()));
		return;
	}
	if (active && !channel->isOp(splited_args[2]))
	{
		try
		{
			Client &target_client = channel->getChanClient(splited_args[2]);
			target_client.addUserMode('@');
			std::cout << "added channel op" << std::endl;
			channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+o" : "-o"), splited_args[2]));
		}
		catch (std::out_of_range &e)
		{
			std::cout << "client not found" << std::endl;
		}
	}
	else if (!active && channel->isOp(splited_args[2]))
	{
		std::cout << "nick :" + splited_args[2] + "//" << std::endl;
		try
		{
			Client &target_client = channel->getChanClient(splited_args[2]);
			target_client.removeUserMode('@');
			std::cout << "removed channel op" << std::endl;
			channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+o" : "-o"), splited_args[2]));
		}
		catch (std::out_of_range &e)
		{
			std::cout << "client not found" << std::endl;
		}
	}
}

void	ModeCommand::mode_invite(Channel* channel, Client& client, bool active, std::vector<std::string> splited_args)
{
	if (splited_args[2].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "OVERRIDE INVITE ONLY"));
		return;
	}
	if (active)
	{
		if (channel->isExceptedFromInvite(splited_args[2]))
		{
			client.reply(ERR_ALREADYEXCEPTEDFROMINVITE(client.getNickname(), splited_args[2], channel->getName()));
			return;
		}
		channel->addExceptionInvite(splited_args[2]);
	}
	else
	{
		if (!channel->isExceptedFromInvite(splited_args[2]))
		{
			client.reply(ERR_ALREADYUNEXCEPTEDFROMINVITE(client.getNickname(), splited_args[2], channel->getName()));
			return;
		}
		channel->removeExceptionInvite(splited_args[2]);
	}
	channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+I" : "-I"), splited_args[2]));
}

void	ModeCommand::mode_exception(Channel* channel, Client& client, bool active, std::vector<std::string> splited_args)
{
	if (splited_args[2].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "OVERRIDE BAN"));
		return;
	}
	if (active)
	{
		if (channel->isExceptedFromBan(splited_args[2]))
		{
			client.reply(ERR_ALREADYEXCEPTEDFROMBAN(client.getNickname(), splited_args[2], channel->getName()));
			return;
		}
		channel->addExceptionBan(splited_args[2]);
	}
	else
	{
		if (!channel->isExceptedFromBan(splited_args[2]))
		{
			client.reply(ERR_ALREADYUNEXCEPTEDFROMBAN(client.getNickname(), splited_args[2], channel->getName()));
			return;
		}
		channel->removeExceptionBan(splited_args[2]);
	}
	channel->broadcastMessage(RPL_MODE(client.getPrefix(), channel->getName(), (active ? "+e" : "-e"), splited_args[2]));
}

void ModeCommand::mode_client(Client *client, std::vector<std::string> splited_args)
{
	// single argument : reply modestring to user
	if (splited_args.size() < 2)
	{
		std::string modes = client->getClientOnServer()->getModes().empty() ? std::string("") : "+" + client->getClientOnServer()->getModes();
		client->reply(RPL_UMODEIS(client->getNickname(), modes));
		return;
	}

	// invisibility mode
	if (splited_args[1] == "+i")
	{
		client->getClientOnServer()->addUserMode('i');
		client->write(":" + client->getPrefix() + " MODE " + client->getNickname() + " +i");
	}
	else if (splited_args[1] == "-i")
	{
		client->getClientOnServer()->removeUserMode('i');
		client->write(":" + client->getPrefix() + " MODE " + client->getNickname() + " -i");
	}

	// unrecognized mode
	else
		client->reply(ERR_UMODEUNKNOWNFLAG(client->getNickname()));

}
