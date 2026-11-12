#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <poll.h>
#include <map>
#include "Client.hpp"
// #include "CommandHandler.hpp"

class Client;

class Server
{
public:
	Server(); //je le met en publique pour pouvoir l'utiliser dans le constructeur de commandhandler
	Server(const Server &other);
	Server&				operator=(const Server &other);
	Server(int port, const std::string &password);
	~Server();

	const std::string&	getPassword() const;
	std::map<int, Client*>	_clients;

	void	_readFromClient(size_t index);
	void	_processLine(Client &client, const std::string &line);
	void	_removeClient(size_t index);

	//j'aurais besoin de ça, ça donnera le type d'erreur a l'user
	//
	// void sendTo(Client &client, const std::string &message);
private:
	// CommandHandler _handler;

	int					_port;
	std::string			_password;
	int					_listenFd;

	void				_setupListenSocket();
	void				_closeListenSocket();

	public:
		void run();
	private:
		std::vector<struct pollfd>	_pollFds;
		void						_acceptNewClient();
};



#endif
