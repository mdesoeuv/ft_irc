#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {

	std::string reason = arguments[1].empty() ? "Quit for unknown reason !" : arguments;
	client.write(RPL_QUIT(client.getPrefix(), reason));
}