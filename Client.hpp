#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
public:
	Client(int fd);
	~Client();

	int			getFd() const;

	std::string	recvBuffer;
	std::string	sendBuffer;

private:
	Client();
	Client(const Client &other);
	Client&	operator=(const Client &other);

	int			_fd;
};

#endif