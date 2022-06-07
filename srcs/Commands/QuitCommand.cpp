#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {
	std::cout << "Client has Quit " << std::endl;
	std::string reason = arguments.empty() ? "Quit for unknown reason !" : arguments;
	_server->allChannelLeave(client);
	client.write(RPL_QUIT(client.getPrefix(), reason));
}