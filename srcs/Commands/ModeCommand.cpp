#include "../../inc/Command.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server) {}

ModeCommand::~ModeCommand() {}

// uniquement channel mode pour le moment 
void ModeCommand::execute(Client& client, std::string arguments) {

	std::vector<std::string> splited_args;
	split_args(arguments, " ", splited_args);

	if (splited_args.size() < 2 || splited_args[0].empty() || splited_args[1].empty())
	{
		client.reply(ERR_CMDNEEDMOREPARAMS(client.getNickname(), "PRIVMSG"));
		return;
	}

	std::string target = splited_args[0];
	Channel channel;
	try
	{
		channel = _server->getChannel(target);
	}
	catch(const std::out_of_range& e)
	{
		client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), target));
		return ;
	}

/*
	if (//check if client is channel operator) {
		client.reply(ERR_CHANOPRIVSNEEDED(client.getNickname(), target));
		return;
	}*/

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