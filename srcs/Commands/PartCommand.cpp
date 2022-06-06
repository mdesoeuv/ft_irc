#include "../../inc/Command.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client& client, std::string arguments) {

  // arguments parsing to extract channel_name
  size_t pos = arguments.find(" ");
  std::string sub_arguments = arguments.substr(0, pos);

  // check if channel exists and proceed
std::pair<bool, std::vector<Channel>::iterator> result = _server->searchChannel(sub_arguments);
	if (result.first)
	{
      result.second->broadcastMessage(":" + client.getPrefix() + " PART " + arguments);
			result.second->delUser(client);
      if (result.second->getUserList().empty())
        _server->removeChannel(result.second);
      return ;
	}
  client.write(ERR_NOSUCHCHANNEL(client.getNickname(), sub_arguments));
}
