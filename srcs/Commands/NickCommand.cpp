#include "../../inc/Command.hpp"

NickCommand::NickCommand(Server *server) : Command(server) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client& client, std::string arguments) {
	if (arguments.empty()) {
		client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()));
		return;
	}

	if (_server->getClient(arguments)) {
		client.reply(ERR_NICKNAMEALREADYUSED(client.getNickname()));
		return;
	}
	//TODO: interdire le nickname 'anonymous' et autre noms impossibles VICTOR
	//if (arguments.compare(anonymous) && arguments[0] == '$' && arguments[0] == ':' && arguments[0] == '+' && arguments[0] == '=')
	_server->allChannelBroadcast(client.getNickname(), CHANGEDNICKNAME(client.getPrefix(), arguments));
	_server->allChannelChangeNickname(client.getNickname(), arguments);
	client.setNickname(arguments);
	std::cout << "Client Nickname set : " << client.getNickname() << std::endl;
	client.welcome();
}