#include "../../inc/Command.hpp"

NamesCommand::NamesCommand(Server *server) : Command(server) {}

NamesCommand::~NamesCommand() {}

void NamesCommand::execute(Client& client, std::string arguments) {

  std::cout << "arguments to parse :" + arguments << std::endl;
  try
  {
    Channel& channel = _server->getChannel(arguments);

    // checks if channel's 'secret mode' is enabled and client not on channel
    if (!channel.isUser(client.getNickname()) && channel.isMode('s'))
      client.reply(RPL_ENDOFNAMES(client.getNickname(), arguments));
    
    // reply userlist without invisible users
    else if (!channel.isUser(client.getNickname()))
    {
      client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getSymbol(), channel.getName(), channel.getUserList(false)));
    }
    
    // reply full userlist
    else
      client.reply(RPL_NAMEREPLY(client.getNickname(), channel.getSymbol(), channel.getName(), channel.getUserList(true)));
  }
  catch(const std::exception& e)
  {
    std::cout << "this channel does not exist" << std::endl;
    client.reply(RPL_ENDOFNAMES(client.getNickname(), arguments));
  }
  
}
