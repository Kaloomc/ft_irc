#include "Server.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <csignal>
#include <iostream>

static volatile sig_atomic_t g_stop = 0;

static void handleSignal(int)
{
	g_stop = 1;
}

Server::Server(int port, const std::string &password)
	: _port(port), _password(password), _listenFd(-1)
{
	try
	{
		_setupListenSocket();
	}
	catch (...)
	{
		// Si le constructeur lance une exception, le destructeur n'est
		// JAMAIS appelé : on ferme donc le fd nous-mêmes avant de relancer.
		_closeListenSocket();
		throw;
	}
}

Server::~Server()
{
	for (size_t i = 0; i < _pollFds.size(); ++i)
	if (_pollFds[i].fd != _listenFd)
		close(_pollFds[i].fd);
	_closeListenSocket();
}

const std::string& Server::getPassword() const
{
	return _password;
}

void Server::_closeListenSocket()
{
	if (_listenFd >= 0)
	{
		close(_listenFd);
		_listenFd = -1;
	}
}

void Server::_setupListenSocket()
{
	// 1. Création du socket : IPv4 (AF_INET), flux TCP (SOCK_STREAM)
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd < 0)
		throw std::runtime_error("socket() failed");

	// 2. SO_REUSEADDR : on peut relancer le serveur tout de suite
	int opt = 1;
	if (setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt() failed");

	// 3. Non-bloquant (seul flag autorisé par le sujet)
	if (fcntl(_listenFd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");

	// 4. bind : on attache le socket au port, sur toutes les interfaces
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(_port);

	if (bind(_listenFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed (port already in use?)");

	// 5. listen : le socket devient un socket d'écoute
	if (listen(_listenFd, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed");
}

void Server::run()
{
	signal(SIGINT, handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGPIPE, SIG_IGN);

	struct pollfd listenPfd;
	listenPfd.fd = _listenFd;
	listenPfd.events = POLLIN;
	listenPfd.revents = 0;
	_pollFds.push_back(listenPfd);

	while (!g_stop)
	{
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ret < 0)
		{
			if (g_stop)
				break;
			throw std::runtime_error("poll() failed");
		}
		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			short re = _pollFds[i].revents;
			if (re == 0)
				continue;
			if (_pollFds[i].fd == _listenFd)
			{
				if (re & POLLIN)
					_acceptNewClient();
				continue;
			}
			if (re & (POLLERR | POLLNVAL))
				_removeClient(i--);
			else if (re & POLLIN)
				_readFromClient(i);
			else if (re & POLLHUP)
				_removeClient(i--);
		}
	}
}

void Server::_acceptNewClient()
{
	struct sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);

	int clientFd = accept(_listenFd, (struct sockaddr *)&clientAddr, &len);
	if (clientFd < 0)
		return;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(clientFd);
		return;
	}
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
	_clients[clientFd] = new Client(clientFd);
	std::cout << "New client connected, fd " << clientFd << std::endl;
}

void Server::_readFromClient(size_t index)
{
	int		fd = _pollFds[index].fd;
	Client	*client = _clients[fd];
	char	buf[512];

	ssize_t n = recv(fd, buf, sizeof(buf), 0);
	if (n <= 0)
	{
		_removeClient(index);
		return;
	}
	client->recvBuffer.append(buf, n);

	size_t pos;
	while ((pos = client->recvBuffer.find('\n')) != std::string::npos)
	{
		std::string line = client->recvBuffer.substr(0, pos);
		client->recvBuffer.erase(0, pos + 1);
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (!line.empty())
			_processLine(*client, line);
	}
	if (client->recvBuffer.size() > 4096)   // pas de \n : client suspect
		_removeClient(index);
}

void Server::_processLine(Client &client, const std::string &line)
{
	std::cout << "[fd " << client.getFd() << "] line: '" << line << "'" << std::endl;
}

void Server::_removeClient(size_t index)
{
	int fd = _pollFds[index].fd;

	std::cout << "Client disconnected, fd " << fd << std::endl;
	close(fd);
	delete _clients[fd];
	_clients.erase(fd);
	_pollFds.erase(_pollFds.begin() + index);
}