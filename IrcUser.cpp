#include "IrcUser.hpp"

IrcUser::IrcUser() : passwordAccepted(false), hasNickname(false), hasUsername(false), registered(false)
{
}

IrcUser::IrcUser(const IrcUser& other)
{
    *this = other;
}

IrcUser& IrcUser::operator=(const IrcUser& other)
{
    if (this != &other)
    {
		this->passwordAccepted = other.passwordAccepted;
		this->hasNickname = other.hasNickname;
		this->hasUsername = other.hasUsername;
		this->registered = other.registered;
    }
    return *this;
}

IrcUser::~IrcUser()
{
}
