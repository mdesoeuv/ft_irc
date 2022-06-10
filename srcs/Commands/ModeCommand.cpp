#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

// uniquement channel mode pour le moment 
void ModeCommand::execute(Client& client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	// TODO: reply de la commande MODE #channel
	
	// checks if a target and a parameter are present
	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PRIVMSG")); //TODO: verifier si PRIVMSG ou MODE
		return;
	}

	std::string target = splited_args[0];
	
	std::cout << "target :" + target + "//" << std::endl;
	// check if target is a channel then if client is op and then execute operation
	if (target[0] == '#')
	{
		
		try
		{
			Channel& channel = _server->getChannel(target);
			std::cout << "channel found" << std::endl;
			if (!channel.isOp(client.getNickname()))
			{
				client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), target));
				return ;
			}
			mode_channel(channel, client, splited_args);
		}
		catch(const std::out_of_range& e)
		{
			std::cout << e.what() << std::endl;
			client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target));
			return ;
		}
	}
	else
	{
		// checks if user exists and execute operation

		Client* target_client = _server->getClient(target);
		if (target_client != nullptr)
			mode_client(target_client, splited_args);
		else
			client.reply(ERR_NOSUCHNICK(client.getNickname(), target));
	}


}

void	ModeCommand::mode_channel(Channel& channel, Client& client, std::vector<std::string> splited_args) {

	//DEBUG display 
	std::cout << "executing Mode command :";
	for (std::vector<std::string>::iterator it = splited_args.begin(); it != splited_args.end(); ++it) {
		std::cout << *it + " ";
	}
	std::cout << std::endl;

	int i = 0;
	int p = 2;
	char c;

	while ((c = splited_args[1][i])) {

		char prevC = i > 0 ? splited_args[1][i - 1] : '\0';
		bool active = prevC == '+';

		switch (c) {

			case 'n': {
				//channel.setNoExt(active);
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+n" : "-n"), ""));
				break;
			}

			case 'o': {
				std::cout << "active :" << active << std::endl;
				if (splited_args.size() != 3)
				{
					client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "MODE"));
					return ;
				}
				if (!channel.isUser(splited_args[2]))
				{
					client.reply(ERR_USERNOTINCHANNEL(client.getNickname(), splited_args[2], channel.getName()));
					return ;
				}
				if (active && !channel.isOp(splited_args[2]))
				{
					try {
						Client&	target_client = channel.getChanClient(splited_args[2]);
						target_client.addUserMode('o');
						std::cout << "added channel op" << std::endl;
						channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+o" : "-o"), splited_args[2]));
					}
					catch(std::out_of_range& e)
					{
						std::cout << "client not found" << std::endl;
					}
				}
				else if (!active && channel.isOp(splited_args[2]))
				{
					std::cout << "nick :" + splited_args[2] + "//" << std::endl;
					try {
						Client&	target_client = channel.getChanClient(splited_args[2]);
						target_client.removeUserMode('o');
						std::cout << "removed channel op" << std::endl;
						channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+o" : "-o"), splited_args[2]));
					}
					catch(std::out_of_range& e)
					{
						std::cout << "client not found" << std::endl;
					}
				}
				else
					return ;
				return ;
				break;
			}

			case 'l': {
				//channel.setMaxClients(active ? std::stol(splited_args[p]) : 0);
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+l" : "-l"), (active ? splited_args[p] : "")));
				p += active ? 1 : 0;
				break;
			}

			case 'k': {
				//channel.setPassword(active ? splited_args[p] : "");
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+k" : "-k"), (active ? splited_args[p] : "")));
				p += active ? 1 : 0;
				break;
			}

			default:
				break;
		}

		i++;
	}
}

void	ModeCommand::mode_client(Client* client, std::vector<std::string> splited_args) {
	
	(void)client;
	std::cout << "executing Mode command :";
	for (std::vector<std::string>::iterator it = splited_args.begin(); it != splited_args.end(); ++it) {
		std::cout << *it + " ";
	}
	std::cout << std::endl;
}