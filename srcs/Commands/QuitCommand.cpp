#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {
	arguments = arguments.substr(1, arguments.size());
	std::string reason = arguments.empty() ? "Quit for unknown reason !" : arguments;
	//TODO: anonymise if channel.isMode('a') => send a PART msg instead of quit -> reason = "" ?
	_server->allChannelLeave(client, RPL_QUIT(client.getPrefix(), reason));
	client.write(RPL_QUIT(client.getPrefix(), reason));
	_server->addClientToDelete(client.getSocketfd());
	std::cout << "Client has Quit " << std::endl;
}