#include "../inc/IrcBot.hpp"

IrcBot::IrcBot(const std::string &host, const std::string &port, const std::string &password) :
		_host(host), _port(port), _password(password), _serverPrefix("ft_irc@127.0.0.1") {
	_sock = newSocket();
}

IrcBot::~IrcBot() {
}

int IrcBot::newSocket() {

	/* creating socket :
	 * domain : AF_INET -> Socket using IPV4
	 * type : SOCK_STREAM : Dialogue support guaranteeing integrity, providing a binary data stream, and integrating a mechanism for out-of-band data transmissions.
	 * protocol : 0 indicates that the caller does not want to specify the protocol and will leave it up to the service provider.
	*/
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");

	// Creating serv_address, giving the parameters to the struct then biding it to the socket
	struct sockaddr_in serv_address = {};

	// Clear address structure, should prevent some segmentation fault and artifacts
	bzero((char *) &serv_address, sizeof(serv_address));

	serv_address.sin_family = AF_INET; // Socket using IPV4
	serv_address.sin_addr.s_addr = inet_addr(_host.c_str());
	serv_address.sin_port = htons(std::stoi(_port)); // TCP protocol does not read a port int so we use htons() to convert unsigned short int to big-endian network byte order as expected from TCP protocol standards

	// Bind the socket to the current IP address on selected port
	if (connect(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		throw std::runtime_error("Error while connecting to host.");

	return sockfd;
}

void IrcBot::start() {

	bool g_BotRunning = true;
	pollfd bot_fd = {_sock, POLLIN | POLLOUT, 0};
	_pollfds.push_back(bot_fd);
	authenticate("BotDePaille");
	while (g_BotRunning)
	{
		std::cout << "send queue :" << _sendQueue << std::endl;
		// poll loop
		if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
			if (g_BotRunning)
				throw std::runtime_error("Error while polling from fd.");
		pollfds_iterator it = _pollfds.begin();
		if (it->revents & POLLHUP)
		{
			g_BotRunning = false; // server disconnect -> shutdown du bot
			break;
		}

		if (it->revents & POLLIN) // POLLIN: le bot reçoit un message
		{
			onServerMessage(it->fd);
		}

		// POLLOUT: le bot peut send //  TODO: 
		if (it->revents & POLLOUT)
		// if (!_sendQueue.empty())
		{
			sendMessageToServer();
		}

		//server POLLERR -> g_BotRunning = false
		if (it->revents & POLLERR)
		{
			g_BotRunning = false;
		}
	}
	std::cout << "Terminating Bot !" << std::endl;
	// addSendQueue("QUIT :Bye !");
	close(_sock);
}



void IrcBot::addSendQueue(const std::string& message) {
	
	std::string full_message = message;
	if (full_message.size() > MSG_SIZE_LIMIT)
		full_message.resize(MSG_SIZE_LIMIT - 2);
	full_message += "\r\n";
	_sendQueue += full_message;
	std::cout << "added to send queue :" + full_message << std::endl;
	sendMessageToServer();
}

void IrcBot::sendMessageToServer()
{
	std::cout << "Trying to send message" << std::endl;
	if (_sendQueue.empty())
		return;
	int sent_bytes = send(_sock, _sendQueue.c_str(), _sendQueue.length(), 0);
	if (sent_bytes < 0)
		std::cout << "Error while sending message to server." << std::endl;
	std::cout << "message sent :" + _sendQueue.substr(0, sent_bytes) << std::endl;
	_sendQueue.erase(0, sent_bytes);
}


void IrcBot::sendPrivMsg(const std::string &source, const std::string &message) {
	addSendQueue("PRIVMSG " + source + " :" + message);
	sendMessageToServer();
}

void IrcBot::onMessageReceived(const std::string &message) {

	std::vector<std::string> splited_args;
	split_args(message, " ", splited_args);

	if (splited_args.size() < 2)
		return;

	std::string source = splited_args.at(0);
	std::string type = splited_args.at(1);
}

std::string	IrcBot::extractMessage() {

	std::string message; 

	size_t pos = _messageBuffer.find("\r\n");
	message = _messageBuffer.substr(0, pos);
	_messageBuffer.erase(0, pos + 2);

	std::cout << "extracted message :" + message << std::endl;
	return message;
}


// command[0]: sender, command[1]: command, command[2]: argument
void IrcBot::onServerMessage(int fd)
{
	int read_bytes = -10;
	char buffer[BUFFER_SIZE + 1];

	bzero(buffer, BUFFER_SIZE + 1);
	while (read_bytes != 0)
	{
		bzero(buffer, BUFFER_SIZE);
		read_bytes = recv(fd, buffer, BUFFER_SIZE, 0);
		if (read_bytes < 0)
			break;
		std::cout << "bytes read :" << read_bytes << std::endl;
		buffer[read_bytes] = '\0';
		std::cout << "packet received :" + std::string(buffer) + "//" << std::endl;
		_messageBuffer.append(buffer);
		while (_messageBuffer.find("\r\n") < _messageBuffer.size())
		{
			parseExecute(extractMessage());
		}
	}
}

void IrcBot::authenticate(const std::string &nickname) {
	addSendQueue("PASS " + _password);
	addSendQueue("NICK " + nickname);
	addSendQueue("USER botDePaille 0 * :Custom ft_irc's bot");
}

void	IrcBot::ParseCommand(std::vector<std::string>& command, std::string message) {
	
	//parse sender
	size_t pos = message.find(" ");
	std::string sender = message.substr(1, pos - 1);
	message.erase(0, pos + 1);
	pos = sender.find("!");
	if (pos < sender.size())
		sender.erase(pos, sender.size());
	command.push_back(sender);

	//parse command
	pos = message.find(" ");
	if (pos > message.size())
	{
		command.push_back(message);
		return;
	}
	command.push_back(message.substr(0, pos));
	message.erase(0, pos + 1);
	
	//parse argument
	pos = message.find(":");
	if (pos > message.size())
	{
		command.push_back(message);
		return;
	}
	command.push_back(message.substr(0, pos - 1));
	command.push_back(message.substr(pos + 1, message.size() - pos - 2));

	//display command
	std::cout << "parsed command :" << std::endl;
	for (std::vector<std::string>::iterator it = command.begin(); it != command.end(); ++it)
	{
		std::cout << *it << "//" << std::endl;
	}
}
std::string IrcBot::whoWins(const std::string& userChoice, const std::string& botChoice) {

	if (botChoice == userChoice)
		return "Duce !";
	if (botChoice == "paper" && userChoice == "rock")
		return "Bot de paille wins !";
	if (botChoice == "rock" && userChoice == "scissors")
		return "Bot de paille wins !";
	if (botChoice == "scissors" && userChoice == "paper")
		return "Bot de paille wins !";
	return "You win !";

}


void IrcBot::rockPaperScissors(const std::vector<std::string>& command) {
	std::string response = "";
	std::string options [] = { "paper", "rock", "scissors"};
	std::string result = options[std::rand() % 3];
		if ((command[3].find("!paper") < command[3].size()) && !(command[3].find("!rock") < command[3].size()) && !(command[3].find("!scissors") < command[3].size()))
			response = "You chose paper Bot de Paille chose " + result + ". " + whoWins("paper", result);
		else if (!(command[3].find("!paper") < command[3].size()) && (command[3].find("!rock") < command[3].size()) && !(command[3].find("!scissors") < command[3].size()))
			response = "You chose rock Bot de Paille chose " + result + ". " + whoWins("rock", result);
		else if (!(command[3].find("!paper") < command[3].size()) && !(command[3].find("!rock") < command[3].size()) && (command[3].find("!scissors") < command[3].size()))
			response = "You chose scissors Bot de Paille chose " + result + ". " + whoWins("scissors", result);
		else
			response = "Veuillez envoyer !paper, !rock ou !scissors pour jouer contre Bot De Paille.";
	sendPrivMsg(command[0], response);
}


void IrcBot::parseExecute(const std::string& message) {

	std::vector<std::string> command;
	ParseCommand(command, message);

	if (command.size() == 0)
		return;
	if (command[1] == "PING")
		addSendQueue("PONG :" + command[0]);
	else if (command[1] == "PRIVMSG")
		rockPaperScissors(command);
	else
		std::cout << "Message type unknown : " + command[1] + " sent by " << command[0] << std::endl;
}
