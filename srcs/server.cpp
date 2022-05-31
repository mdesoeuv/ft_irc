#include "../inc/server.hpp"

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

		// poll est une focntion qui boucle jusqu'à l'arrivée de nouvelles data
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			//error

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

	int sockfd = socket();
	//creation du socket puis régalge du socket en non bloquant avec fcntl puis bind du socket sur l'addresse et le port précisé puis listen
	return sockfd;
}

void Server::onClientConnect() {
//ajouter le fd du nouveau client dans le poll
}

void Server::onClientDisconnect(int fd) {
//retirer le fd du poll
}

void Server::onClientMessage(int fd) {
//readMsg
}

std::string Server::readMessage(int fd) {

	// lire le message du fd avec la fonction recv
}

Client *Server::getClient(const std::string &nickname) {
	// parcourir l'iterator client pour récuperer le bon s'il existe 
}
