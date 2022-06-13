#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute(Client& client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	// checks if a target and a parameter are present
	if (splited_args.size() < 1 || splited_args[0].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PRIVMSG")); //TODO: verifier si PRIVMSG ou MODE
		return;
	}

	std::string target = splited_args[0];
	
	std::cout << "target of mode command:" + target + "//" << std::endl;
	// check if target is a channel then if client is op and then execute operation
	if (target[0] == '#')
	{
		try
		{
			Channel& channel = _server->getChannel(target);
			// processing MODE #channel command without parameters
			if (splited_args.size() == 1)
			{
				client.reply(RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), channel.getModes(), ""));
				return ;
			}
			// verify if client is op to process other types of MODE cmd
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

//TODO: faire des sous fonctions pour chaque cas du switch
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

			case 'm': {
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

			case 'n': {
				//channel.setNoExt(active);
				channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+n" : "-n"), ""));
				break;
			}

			// operator mode: @ before nickname 
			case 'o': { // OK
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
						target_client.addUserMode('@');
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
						target_client.removeUserMode('@');
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

			// voice mode on channel: '+' before nick
			case 'v': {
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
						target_client.addUserMode('+');
						std::cout << "added voice mode to " + target_client.getNickname() + " for channel " + channel.getName() << std::endl;
					}
					channel.broadcastMessage(RPL_MODE(client.getPrefix(), channel.getName(), (active ? "+v" : "-v"), (active ? splited_args[p] : "")));
				}
				catch (std::out_of_range& e)
				{
					std::cout << "Client not found" << std::endl;
					break ;
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

void	ModeCommand::mode_client(Client* client, std::vector<std::string> splited_args) {
	
	(void)client;
	std::cout << "Client Modes not supported :";
	for (std::vector<std::string>::iterator it = splited_args.begin(); it != splited_args.end(); ++it) {
		std::cout << *it + " ";
	}
	std::cout << std::endl;
}

// void	ModeCommand::voice_mode(Client)