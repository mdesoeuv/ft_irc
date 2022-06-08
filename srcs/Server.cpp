#include "../inc/Server.hpp"
#include "../inc/CommandHandler.hpp"

Server::Server(const std::string port, const std::string password)
		: _running(1), _host("127.0.0.1"), _port(port), _password(password) {

	_commandHandler = new CommandHandler(this);
	_sock = newSocket();
}

Server::~Server() {
	delete _commandHandler;
}

void Server::start() {
	pollfd server_fd = {_sock, POLLIN, 0};
	_pollfds.push_back(server_fd);

	// Le server écoute désormais les POLL IN
	while (_running) {

		// poll est une fonction qui boucle jusqu'à l'arrivée de nouvelles data
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			throw std::runtime_error("Error while polling from fd.");
		//  Un des fd a un nouveau message, on les parcourt pour savoir lequel
		for (pollfds_iterator it = _pollfds.begin() ;it != _pollfds.end(); ++it) {

			if (it->revents == 0)
				continue;

			if (it->revents == POLLHUP) {
				onClientDisconnect(it->fd);
				break;
			}

			if (it->revents == POLLIN) {

				if (it->fd == _sock) {
					onClientConnect();
					break;
				}

				onClientMessage(it->fd);
			}
		}
	}
}

int Server::newSocket() {

	/* creating socket :
	 * domain : AF_INET -> Socket using IPV4
	 * type : SOCK_STREAM : Dialogue support guaranteeing integrity, providing a binary data stream, and integrating a mechanism for out-of-band data transmissions.
	 * protocol : 0 indicates that the caller does not want to specify the protocol and will leave it up to the service provider.
	*/
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");

	/*Adding option to socket :
	 * Socket Layer : SOL_SOCKET : Means we modify the socket itslef
	 * option : SO_REUSEADDR : Forcefully attaching socket to the port
	 * value : 1 for forcing socket to use port given
	 */
	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");

	/* Setting the socket to NON-BLOCKING mode allowing it to return any data that the system has in it's read buffer
	 * for that socket even if the fd is still in use. It won't wait for that data to be terminated and will send an error.
	 * command : F_SETFL : setting for state attribute of fd
	 * arg : O_NONBLOCK meaning described previously 
	 */
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) {
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	}

	// Creating serv_address, giving the parameters to the struct then biding it to the socket
	struct sockaddr_in serv_address = {};

	// Clear address structure, should prevent some segmentation fault and artifacts
	bzero((char *) &serv_address, sizeof(serv_address));

	serv_address.sin_family = AF_INET; // Socket using IPV4
	serv_address.sin_addr.s_addr = INADDR_ANY; // means can attached socket to any address
	serv_address.sin_port = htons(std::stoi(_port)); // TCP protocol does not read a port int so we use htons() to convert unsigned short int to big-endian network byte order as expected from TCP protocol standards

	// Bind the socket to the IP address and port
	if (bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		throw std::runtime_error("Error while setting socket IP address and port.");

	// Define max connexions and let socket be able to listen for requests
	if (listen(sockfd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening on socket.");
	return sockfd;
}

void Server::onClientConnect() {

	// adding new fd to poll 
	
	int fd;
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	fd = accept(_sock, (sockaddr *) &s_address, &s_size);
	if (fd < 0)
		throw std::runtime_error("Error while accepting new client.");

	pollfd pollfd = {fd, POLLIN, 0};
	_pollfds.push_back(pollfd);

	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname of new client.");

	// Creates a new Client and store it in Clients map
	_clients.insert(std::make_pair(fd, Client(fd, hostname, ntohs(s_address.sin_port), "")));
	std::cout << "Client connnected" << std::endl;
}

void Server::onClientDisconnect(int fd) {

	// client is removed from all subscribed channels
	allChannelLeave(_clients[fd], ":" + _clients[fd].getPrefix() + " QUIT :Quit:");

	// removing fd of leaving client from poll 
	deleteClient(fd);
}

void Server::onClientMessage(int fd) {
	std::cout << "Client message !" << std::endl;
	readMessage(fd);
}

void Server::readMessage(int fd) {

	int			read_bytes = -10;
	char		buffer[101];
	
	bzero(buffer, 101);
	while (read_bytes != 0)
	{
		bzero(buffer, 100);
		read_bytes = recv(fd, buffer, 100, 0);
		if (read_bytes < 0)
			break ;
		std::cout << "bytes read :" << read_bytes << std::endl;
		buffer[read_bytes] = '\0';
		std::cout << "packet received :" + std::string(buffer) + "//" << std::endl;
		_clients[fd].getMessageBuffer().append(buffer);
		while (_clients[fd].getMessageBuffer().find("\r\n") < _clients[fd].getMessageBuffer().size())
		{
			_commandHandler->parsing(_clients[fd], _clients[fd].extractMessage());
		}
	}
}

Client *Server::getClient(const std::string nickname) {
	
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (!nickname.compare(it->second.getNickname()))
			return &it->second;
	}
	return nullptr;
}

Channel&	Server::getChannel(const std::string& channel_name) {

	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->getName() == channel_name)
			return *it;
	}
	throw std::out_of_range("channel does not exist");
}

void	Server::deleteClient(int fd) {
	_clients.erase(fd);
	for (pollfds_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it) {
		if (it->fd == fd)
		{
			_pollfds.erase(it);
			close(fd);
			break;
		}			
	}
}

void	Server::addChannel(Channel channel) {
	_channels.push_back(channel);
}

void	Server::removeChannel(std::vector<Channel>::iterator pos) {
	_channels.erase(pos);
}


std::pair<bool, std::vector<Channel>::iterator>	Server::searchChannel(const std::string channel_name) {
	
	std::vector<Channel>::iterator iter = _channels.begin();

	for(; iter != _channels.end(); ++iter)
	{

		if (iter->getName() == channel_name)
		{
			return std::make_pair(true, iter);
		}
	}
	return std::make_pair(false, iter);
}

void	Server::allChannelLeave(Client client, std::string broadcast_message) {

	std::vector<std::string> channels_to_remove;

	for (std::vector<Channel>::iterator chan_iter = _channels.begin(); chan_iter != _channels.end(); ++chan_iter)
	{
		if (chan_iter->isUser(client.getNickname()))
		{
			chan_iter->broadcastMessage(broadcast_message);
			chan_iter->delUser(client);
			if (chan_iter->getUserList().empty())
				channels_to_remove.push_back(chan_iter->getName()); // cannot remove in loop because iterator is invalidated by each removal
		}
	}
	for (std::vector<std::string>::iterator iter = channels_to_remove.begin(); iter != channels_to_remove.end(); ++iter)
	{
		removeChannel(this->searchChannel(*iter).second);
	}
}