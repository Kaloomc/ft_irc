#include "Server.hpp"
#include <iostream>
#include <stdlib.h> 

int main(int ac, char** av)
{
	if (ac != 3 || (atoi(av[1]) < 1 && atoi(av[1]) > 65535))
		return 0;
	try
	{ 
		Server s(atoi(av[1]), av[2]);
		s.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl; return 1;
	}
}