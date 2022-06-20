#include "../../inc/Command.hpp"

QuitCommand::QuitCommand(Server *server, bool auth) : Command(server, auth) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client& client, std::string arguments) {
	(void)client;
	std::string reason = arguments.empty() ? "Quit for unknown reason !" : arguments;
	std::cout << "Client has Quit " << std::endl;
}