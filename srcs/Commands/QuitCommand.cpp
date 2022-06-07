#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {
	arguments = arguments.substr(1, arguments.size());
	std::string reason = arguments.empty() ? "Quit for unknown reason !" : arguments;
	client.write(RPL_QUIT(client.getPrefix(), reason));
	// _server->allChannelLeave(client, false); //segfault
	std::cout << "Client has Quit " << std::endl;
	_server->deleteClient(client.getSocketfd());
}