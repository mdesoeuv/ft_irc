#include "../inc/Server.hpp"

Server::Server(const std::string &port, const std::string &password)
		: _running(1), _host("127.0.0.1"), _port(port), _password(password) {

	_sock = newSocket();
}

Server::~Server() {
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
		pollfds_iterator it = _pollfds.begin();
		while (it++ != _pollfds.end()) {

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

	// Creates a new Client and store it in Clients map
	addClient(fd); // on peut check le bool retourné pour verifier l'ajout
}

void Server::onClientDisconnect(int fd) {

	// removing fd of leaving client from poll 
	deleteClient(fd);
	try {
		pollfds_iterator it = _pollfds.begin();
		while (it++ != _pollfds.end()) {
			if (it->fd == fd)
			{
				_pollfds.erase(it);
				close(fd);
				break;
			}			
		}
	}
	catch (const std::out_of_range &ex) { // je ne pense pas que le code du dessus throw
	}
	
	
}

void Server::onClientMessage(int fd) {
	try {
		// getting which client has sent the msg by finding the fd in de client list 
		Client myclient = _clients.at(fd);
		//TO DO : we have client and message (readMessage(fd)), what do we do next ?
	}
	catch (const std::out_of_range &ex) {
	}
}

std::string Server::readMessage(int fd) {

	std::string message;

	char buffer[100];
	bzero(buffer, 100);

	while (!std::strstr(buffer, "\r\n")) {
		bzero(buffer, 100);

		if (recv(fd, buffer, 100, 0) < 0) {
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while reading buffer from client.");
		}

		message.append(buffer);
	}

	return message;
}

Client& Server::getClient(const std::string &nickname) {
	
	clients_iterator iter = _clients.begin();

	for (; iter != _clients.end(); ++iter)
	{
		if (iter->second.getNickname() == nickname)
			return iter->second;
	}
	throw std::out_of_range("Client not found.");
}

bool	Server::addClient(int fd) {
	return _clients.insert(std::make_pair(fd, Client(fd, "default_nickname"))).second;
}

void	Server::deleteClient(int fd) {
	_clients.erase(fd);
}