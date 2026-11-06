#include "Client.hpp"

Client::Client(int fd) : _fd(fd)
{
}

Client::~Client()
{
	// Le fd n'est PAS fermé ici : c'est Server qui le ferme (dans
	// _removeClient et dans son destructeur), pour éviter un double close().
}

int Client::getFd() const
{
	return _fd;
}