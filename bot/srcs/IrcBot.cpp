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
		// poll loop
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
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

		// POLLOUT: le bot peut send
		if (it->revents & POLLOUT)
		{
			sendMessageToServer(_clients[it->fd]);
		}

		//server POLLERR -> g_BotRunning = false
		if (it->revents & POLLERR)
		{
			g_BotRunning = false;
		}
	}
	// addSendQueue("QUIT :Bye !");
	close(_sock);
}



void IrcBot::addSendQueue(const std::string& message) {
	
	std::string full_message = message;
	if (full_message.size() > MSG_SIZE_LIMIT)
		full_message.resize(MSG_SIZE_LIMIT - 2);
	full_message += "\r\n";
	_sendQueue += full_message;
}

void IrcBot::sendMessageToServer()
{

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
}

void IrcBot::onMessageReceived(const std::string &message) {

	std::vector<std::string> splited_args;
	split_args(message, " ", splited_args);

	if (splited_args.size() < 2)
		return;

	std::string source = splited_args.at(0);
	std::string type = splited_args.at(1);

	//replyToCommand(source, type, std::vector<std::string>(splited_args.begin() + 2, splited_args.end())); TODO
}

std::string	IrcBot::extractMessage() {

	std::string message; 

	size_t pos = _messageBuffer.find("\r\n");
	message = _messageBuffer.substr(0, pos);
	_messageBuffer.erase(0, pos + 2);

	std::cout << "extracted message :" + message << std::endl;
	return message;
}

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
			parseExecute(_messageBuffer.extractMessage());
		}
	}
}

void IrcBot::authenticate(const std::string &nickname) {
	addSendQueue("PASS " + _password);
	addSendQueue("NICK " + nickname);
	addSendQueue("USER botDePaille 0 * :Custom ft_irc's bot");
}

std::string	IrcBot::ParseSender(const std::string& message) {
	size_t pos = message.find(" ");
	std::string sender = message.substr(1, pos);

	return sender;
}

void IrcBot::parseExecute(const std::string& message) {

	std::string sender = ParseSender(message);
	std::string response = "Coucou c'est moi le bot de paille !";
	if (sender != _serverPrefix)
		sendPrivMsg(sender, response);
}
