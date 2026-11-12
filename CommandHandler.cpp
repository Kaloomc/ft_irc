#include "CommandHandler.hpp"
#include "IrcUser.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <cctype>

// CommandHandler::CommandHandler(): _server()
// {
// }

CommandHandler::CommandHandler(const CommandHandler& other)
{
    *this = other;
}

CommandHandler& CommandHandler::operator=(const CommandHandler& other)
{
    if (this != &other)
    {
        // Copier les attributs ici
    }
    return *this;
}

CommandHandler::~CommandHandler()
{
}

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype> 

CommandHandler::CommandHandler(Server &server): _server(server)
{

}

void CommandHandler::execute(Client &client, const std::string &line)
{

	Message msg = parsing(line);
	if (!msg.valid)
		return ;
	//un toUpper, les commandes sont pas sensible au majs
	std::transform((msg.command).begin(), (msg.command).end(), (msg.command).begin(), ::toupper);
	if (msg.command == "PASS")
		_handlePass(client, msg);
	else if (msg.command == "NICK")
		_handlePass(client, msg);
	else if (msg.command == "USER")
		_handlePass(client, msg);
	else{} // psa finis
}

void CommandHandler::_handlePass(Client &client, const Message &msg)
{
	int fd = client.getFd();
	IrcUser &user = _users[fd];
	if (user.registered) //pas d'utlisateur connu
	{
		_server.sendTo(client, ":localhost 462 * :You may not reregister\r\n");
		return;
	}
	if (msg.parameters.size() != 1) //mp = NULL
	{
		_server.sendTo(client, ":localhost 461 * PASS :Not enough parameters\r\n");
		return;
	}
	if (msg.parameters[0] != _server.getPassword())
	{
		user.passwordAccepted = false;
		_server.sendTo(client, "localhost 464 * :Password incorrect\r\n");
		return;
	}
	user.passwordAccepted = true;
	_checkRegistration(client);
}

static std::string toLower(const std::string &str)
{
    std::string result = str;

    for (std::string::size_type i = 0; i < result.size(); ++i)
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    return result;
}

void CommandHandler::_handleNick(Client &client, const Message &msg)
{
	int fd = client.getFd();
	IrcUser &user = _users[fd];

	if (user.registered) //changement de pseudo
	{
	}
	if (msg.parameters.size() != 1)
	{
		_server.sendTo(client, ":localhost 431 * :No nickname given\r\n");
		return;
	}
	const std::string &nickname = msg.parameters[0];
	if (nickname.empty())
	{
		_server.sendTo(client, ":localhost 431 * :No nickname given");
		return;
	}
	//un check pour eviter deux Nick identiques dans les utilisateurs
	for (std::map<int, IrcUser>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first != fd && it->second.hasNickname && toLower(it->second.nickname) == toLower(nickname))
		{
			_server.sendTo(client, ":localhost 433 * " + nickname + " :Nickname is already in use\r\n");
			return;
		}
	}
	for (std::string::size_type i = 0; i < nickname.size(); ++i)
    {
		unsigned char c = static_cast<unsigned char>(nickname[i]);

        if (std::isspace(c) || c == ',' || c == '*' || c == '?' || c == '!' || c == '@' || c == ':')
		{
			_server.sendTo(client, ":localhost 432 * " + nickname + " :Erroneous nickname\r\n");
			return;
		}
	}
	user.nickname = nickname;
	user.hasNickname = true;
	_checkRegistration(client);
}

void CommandHandler::_handleUser(Client &client, const Message &msg)
{
	int fd = client.getFd();
	IrcUser &user = _users[fd];

	if (user.registered)
	{
		_server.sendTo(client, ":localhost 462 * :You are already logged in\r\n");
		return;
	}
	if (msg.parameters.size() != 3 || !msg.hasTrailing)
	{
		_server.sendTo(client, ":localhost 461 * :Not enough parameters\r\n");
		return;
	}
	user.username = msg.parameters[0];
	user.realname = msg.trailing;
	user.hasUsername = true;
	_checkRegistration(client);
}

void CommandHandler::_checkRegistration(Client &client)
{
	int fd = client.getFd();
	IrcUser &user = _users[fd];

	if (user.registered)
		return;
	if (user.passwordAccepted && user.hasNickname && user.hasUsername)
	{
		user.registered = true;
	    _server.sendTo(client, ":localhost 001 " + user.nickname + " :Welcome to the IRC Network " + user.nickname + "!" + user.username + "@localhost\r\n");
		std::cout << "Client registered: " << user.nickname << std::endl;
	}
}

void CommandHandler::removeClient(int fd) //penser a appeler lors de la la deconnection
{
    _users.erase(fd);
}
