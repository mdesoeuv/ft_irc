#include "../../inc/Command.hpp"

PartCommand::PartCommand(Server *server) : Command(server) {}

PartCommand::~PartCommand() {}

void PartCommand::execute(Client& client, std::string arguments) {

  std::cout << "part command initiated with args :" + arguments << std::endl;
  // arguments parsing to extract channel_name
  size_t pos = arguments.find(" ");
  std::string sub_arguments = arguments.substr(0, pos);
	for (std::vector<Channel>::iterator it = _server->_channels.begin(); it != _server->_channels.end(); ++it)
	{
		if (it->getName() == sub_arguments)
    {
      it->broadcastMessage(":" + client.getPrefix() + " PART " + it->getName());
			it->delUser(client);
      std::cout << "part command executed" << std::endl;
      return ;
    }
	}
  // Error message if channel is not found
  client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), sub_arguments));
}

/* WIP 

PART message
     Command: PART
  Parameters: <channel>{,<channel>} [<reason>]
The PART command removes the client from the given channel(s). On sending a successful PART command, the user will receive a PART message from the server for each channel they have been removed from. <reason> is the reason that the client has left the channel(s).

For each channel in the parameter of this command, if the channel exists and the client is not joined to it, they will receive an ERR_NOTONCHANNEL (442) reply and that channel will be ignored. If the channel does not exist, the client will receive an ERR_NOSUCHCHANNEL (403) reply and that channel will be ignored.

This message may be sent from a server to a client to notify the client that someone has been removed from a channel. In this case, the message <source> will be the client who is being removed, and <channel> will be the channel which that client has been removed from. Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these multiple-channel PART messages as a series of messages with a single channel name on each. If a PART message is distributed in this way, <reason> (if it exists) should be on each of these messages.

Numeric Replies:

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
Command Examples:

  PART #twilight_zone             ; leave channel "#twilight_zone"

  PART #oz-ops,&group5            ; leave both channels "&group5" and
                                  "#oz-ops".
Message Examples:

  :dan-!d@localhost PART #test    ; dan- is leaving the channel #test

*/