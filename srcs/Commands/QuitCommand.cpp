#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> arguments) {

	std::string reason = arguments[1].empty() ? "Qui for unknown reason !" : "";
	for (std::vector<std::string>::iterator it = arguments.begin() + 1; it != arguments.end(); ++it)
		reason += *it;
	client->write(RPL_QUIT(client->getPrefix(), reason));
}