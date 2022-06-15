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
		try
		{
			Channel &channel = _server->getChannel(target);
			// processing MODE #channel command without parameters
			if (splited_args.size() == 1)
			{
				client.reply(RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), channel.getModes(), ""));
				return;
			}
			// verify if client is op to process other types of MODE cmd
			if (!channel.isOp(client.getNickname()))
			{
				client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), target));
				return;
			}
			if (!channel.isUser(client.getNickname()))
			{
				client.reply(ERR_NOTONCHANNEL(client.getNickname(), channel.getName()));
				return;
			}
			mode_channel(channel, client, splited_args);
		}
		catch (const std::out_of_range &e)
		{
			std::cout << e.what() << std::endl;
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target));
			return;
		}
	}
	else
	{
		// checks if user exists and execute operation
		Client *target_client = _server->getClient(target);
		if (target_client != nullptr)
			mode_client(target_client, splited_args);
		else
			client.reply(ERR_NOSUCHNICK(client.getNickname(), target));
	}
}

// TODO: faire des sous fonctions pour chaque cas du switch
void ModeCommand::mode_channel(Channel &channel, Client &client, std::vector<std::string> splited_args)
{

	// DEBUG display
	std::cout << "executing Mode command :";
	for (std::vector<std::string>::iterator it = splited_args.begin(); it != splited_args.end(); ++it)
	{
		std::cout << *it + " ";
	}
	std::cout << std::endl;

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
				if (active)
				{
					if (!channel.addMode('a'))
						break;
				}
				else
				{
					if (!channel.removeMode('a'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+a" : "-a"), ""));
				break;
			}
			case 'b':
			{
				if (splited_args[2].empty())
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "BAN"));
					break;
				}
				if (active)
				{
					if (channel.isBanned(splited_args[2]))
					{
						client.reply(ERR_ALREADYBANNED(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.addBan(splited_args[2]);
					channel.broadcastMessage(RPL_BANNED(client.getNickname(), splited_args[2], channel.getName()));
				}
				else
				{
					if (!channel.isBanned(splited_args[2]))
					{
						client.reply(ERR_ALREADYUNBANNED(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.removeBan(splited_args[2]);
					channel.broadcastMessage(RPL_UNBANNED(client.getNickname(), splited_args[0], channel.getName()));
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+b" : "-b"), splited_args[2]));
				break;
			}

			case 'e':
			{
				if (splited_args[2].empty())
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "OVERRIDE BAN"));
					break;
				}
				if (active)
				{
					if (channel.isExceptedFromBan(splited_args[2]))
					{
						client.reply(ERR_ALREADYEXCEPTEDFROMBAN(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.addExceptionBan(splited_args[2]);
				}
				else
				{
					if (!channel.isExceptedFromBan(splited_args[2]))
					{
						client.reply(ERR_ALREADYUNEXCEPTEDFROMBAN(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.removeExceptionBan(splited_args[2]);
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+e" : "-e"), splited_args[2]));
				break;
			}

			case 'I':
			{
				if (splited_args[2].empty())
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "OVERRIDE INVITE ONLY"));
					break;
				}
				if (active)
				{
					if (channel.isExceptedFromInvite(splited_args[2]))
					{
						client.reply(ERR_ALREADYEXCEPTEDFROMINVITE(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.addExceptionInvite(splited_args[2]);
				}
				else
				{
					if (!channel.isExceptedFromInvite(splited_args[2]))
					{
						client.reply(ERR_ALREADYUNEXCEPTEDFROMINVITE(client.getNickname(), splited_args[2], channel.getName()));
						return;
					}
					channel.removeExceptionInvite(splited_args[2]);
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+I" : "-I"), splited_args[2]));
				break;
			}

			case 'm':
			{
				if (active)
				{
					if (!channel.addMode('m'))
						break;
				}
				else
				{
					if (!channel.removeMode('m'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+m" : "-m"), ""));
				break;
			}

			case 'q':
			{
				if (active)
				{
					if (!channel.addMode('q'))
						break;
				}
				else
				{
					if (!channel.removeMode('q'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+q" : "-q"), ""));
				break;
			}

			case 'k':
			{
				if (splited_args[2].empty())
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "CHANNEL PASSWORD"));
					break;
				}
				if (active)
				{
					if (!channel.addMode('k'))
						break;
					channel.setPassword(splited_args[2]);
				}
				else
				{
					if (!channel.removeMode('k'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+k" : "-k"), (active ? splited_args[p] : "")));
				p += active ? 1 : 0;
				break;
			}

					case 'n':
			{
				if (active)
				{
					if (!channel.addMode('n'))
						break;
				}
				else
				{
					if (!channel.removeMode('n'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+n" : "-n"), ""));
				break;
			}

			case 'i':
			{
				if (active)
				{
					if (!channel.addMode('i'))
						break;
				}
				else
				{
					if (!channel.removeMode('i'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+i" : "-i"), ""));
				break;
			}

			// operator mode: @ before nickname
			case 'o':
			{
				std::cout << "active :" << active << std::endl;
				if (splited_args.size() != 3)
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
					return;
				}
				if (!channel.isUser(splited_args[2]))
				{
					client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), splited_args[2], channel.getName()));
					return;
				}
				if (active && !channel.isOp(splited_args[2]))
				{
					try
					{
						Client &target_client = channel.getChanClient(splited_args[2]);
						target_client.addUserMode('@');
						std::cout << "added channel op" << std::endl;
						channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+o" : "-o"), splited_args[2]));
					}
					catch (std::out_of_range &e)
					{
						std::cout << "client not found" << std::endl;
					}
				}
				else if (!active && channel.isOp(splited_args[2]))
				{
					std::cout << "nick :" + splited_args[2] + "//" << std::endl;
					try
					{
						Client &target_client = channel.getChanClient(splited_args[2]);
						target_client.removeUserMode('@');
						std::cout << "removed channel op" << std::endl;
						channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+o" : "-o"), splited_args[2]));
					}
					catch (std::out_of_range &e)
					{
						std::cout << "client not found" << std::endl;
					}
				}
				else
					return;
				return;
				break;
			}

			case 'l':
			{
				channel.setChannelLimit(active ? std::stol(splited_args[p]) : 0);
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+l" : "-l"), (active ? splited_args[p] : "")));
				p += active ? 1 : 0;
				break;
			}

			case 'p':
			{
				if (active)
				{
					if (!channel.addMode('p'))
						break;
				}
				else
				{
					if (!channel.removeMode('p'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+p" : "-p"), ""));
				break;
			}

			case 's':
			{
				if (active)
				{
					if (!channel.addMode('s'))
					{
						break;
					}
					channel.setSymbol("@");
				}
				else
				{
					if (!channel.removeMode('s'))
					{
						break;
					}
					channel.setSymbol("=");
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+s" : "-s"), ""));
				break;
			}

			case 't':
			{
				if (active)
				{
					if (!channel.addMode('t'))
						break;
				}
				else
				{
					if (!channel.removeMode('t'))
						break;
				}
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+t" : "-t"), ""));
				break;
			}

			// voice mode on channel: '+' before nick
			case 'v':
			{
				if (splited_args.size() < 3)
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
					return ;
				}
				if (!channel.isUser(splited_args[2]))
				{
					client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), splited_args[2], channel.getName()));
					return ;
				}
				try
				{
					Client&	target_client = channel.getChanClient(splited_args[2]);
					if (active)
					{
						if (target_client.isMode('+'))
							break ;
						target_client.addUserMode('+');
						std::cout << "added voice mode to " + target_client.getNickname() + " for channel " + channel.getName() << std::endl;
					}
					else
					{
						target_client.removeUserMode('+');
						std::cout << "removed voice mode to " + target_client.getNickname() + " for channel " + channel.getName() << std::endl;
					}
					std::cout << "is voice ok ? " << channel.isClientMode(target_client.getNickname(), '+') << std::endl;

					channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+v" : "-v"), target_client.getNickname()));
				}
				catch (std::out_of_range &e)
				{
					std::cout << "Client not found" << std::endl;
					break;
				}
				p += active ? 1 : 0;
				break;
			}

			default:
				break;
		}

		i++;
	}
}

// TODO: user mode invisibility MEHDI
void ModeCommand::mode_client(Client *client, std::vector<std::string> splited_args)
{
	(void)client;
	std::cout << "Client Modes not supported :";
	for (std::vector<std::string>::iterator it = splited_args.begin(); it != splited_args.end(); ++it)
	{
		std::cout << *it + " ";
	}
	std::cout << std::endl;
}
