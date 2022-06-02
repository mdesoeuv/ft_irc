#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

CommandHandler::CommandHandler() {
}

CommandHandler::~CommandHandler() {
}

void CommandHandler::writeClientMsg(Client client, std::string msg) {
	(void) client;
	std::cout << msg << std::endl;
}