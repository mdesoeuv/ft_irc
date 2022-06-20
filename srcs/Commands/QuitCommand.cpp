#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server, bool auth) : Command(server, auth) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {

	std::cout << "arguments :" << arguments << "//" << std::endl;

	std::string reason = arguments.empty() ? "Quit for unknown reason !" : arguments.substr(1, arguments.size());
	_server->allChannelLeave(client, RPL_QUIT(client.getNickname(), reason));
	std::cout << "Client has Quit " << std::endl;
}