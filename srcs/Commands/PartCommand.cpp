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
    // check if user is not on channel
    if (result.second->getUserList().find(client.getNickname()) > result.second->getUserList().size())
    {
      client.reply(ERR_NOTONCHANNEL(client.getNickname(), result.second->getName()));
      return ;
    }
    // delete user and delete channel if last user
    result.second->delUser(client, arguments);
    if (result.second->getUserList().empty())
      _server->removeChannel(result.second);
	}
  else
    client.write(ERR_NOSUCHCHANNEL(client.getNickname(), sub_arguments));
}
