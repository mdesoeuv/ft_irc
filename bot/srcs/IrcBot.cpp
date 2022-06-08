#include "../inc/IrcBot.hpp"

IrcBot::IrcBot(const std::string &host, const std::string &port, const std::string &password) :
		_host(host), _port(port), _password(password) {
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
	authenticate("BotDePaille");

	//Listen for incoming messages from server
	char buffer[256];
	int length;

	while ((length = recv(_sock, buffer, 256, 0)) > 0) {
		buffer[length] = 0;
		onMessageReceived(std::string(buffer, length));
	}

	//End the bot manually by typing exit in the shell (On devrait peut être faire la même chose pour le prgrm irc server ?)
	std::string input;
	getline(std::cin, input);
	while (input != "exit") {
		getline(std::cin, input);
	}
	close(_sock);
	sendMessageToServer("QUIT :Bye !");

}

void IrcBot::sendMessageToServer(const std::string &message) {

	std::cout << "Sending Message : " << message << std::endl;

	std::string terminatedString = message + "\r\n";
	send(_sock, terminatedString.c_str(), terminatedString.size(), 0);
}

void IrcBot::sendPrivMsg(const std::string &source, const std::string &message) {
	sendMessageToServer("PRIVMSG " + source + " :" + message);
}

void IrcBot::authenticate(const std::string &nickname) {
	sendMessageToServer("PASS " + _password);
	sendMessageToServer("NICK " + nickname);
	sendMessageToServer("USER botDePaille 0 * :Custom ft_irc's bot");
}

void IrcBot::onMessageReceived(const std::string &message) {

	std::vector<std::string> splited_args;
	split_args(message, " ", splited_args);

	if (splited_args.size() < 2)
		return;

	std::string source = splited_args.at(0);
	std::string type = splited_args.at(1);

	//replyToCommand(source, type, std::vector<std::string>(splited_args.begin() + 2, splited_args.end())); TO DO
}