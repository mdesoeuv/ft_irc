#include "../../inc/Command.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client& client, std::string arguments) {

  // arguments parsing to extract channel_name
  size_t pos = arguments.find(" ");
  std::string sub_arguments = arguments.substr(0, pos);

  // check if channel exists and proceed
	for (std::vector<Channel>::iterator it = _server->_channels.begin(); it != _server->_channels.end(); ++it)
	{
		if (it->getName() == sub_arguments)
    {
      it->broadcastMessage(":" + client.getPrefix() + " PART " + arguments);
			it->delUser(client);
      if (it->getUserList().empty())
        _server->_channels.erase(it);
      return ;
    }
	}
  client.write(ERR_NOSUCHCHANNEL(client.getNickname(), sub_arguments));
}
