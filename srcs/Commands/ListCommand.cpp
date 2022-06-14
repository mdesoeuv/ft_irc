#include "../../inc/Command.hpp"

ListCommand::ListCommand(Server *server) : Command(server) {}

ListCommand::~ListCommand() {}

void ListCommand::execute(Client& client, std::string arguments) {

  std::cout << "arguments to parse :" + arguments << std::endl;

  client.reply(RPL_LISTSTART(client.getNickname()));
  if (arguments.empty())
  {
    for (std::vector<Channel>::iterator it = _server->getChannelBegin(); it != _server->getChannelEnd(); ++it)
    {
      if (!it->isMode('s'))
        client.reply(RPL_LIST(client.getNickname(), it->getName(), it->getUserNbStr(), it->getTopic()));
    }
  }
  else
  {
    for (std::vector<Channel>::iterator it = _server->getChannelBegin(); it != _server->getChannelEnd(); ++it)
    {
      if (it->getName() == arguments)
        client.reply(RPL_LIST(client.getNickname(), it->getName(), std::string(it->getUserNbStr()), it->getTopic()));
      break ;
    }

  }
  client.reply(RPL_LISTEND(client.getNickname()));

}
