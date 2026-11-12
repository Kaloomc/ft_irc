/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbastiat <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:36:08 by jbastiat          #+#    #+#             */
/*   Updated: 2026/11/12 15:57:13 by jbastiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP
# include <map>
# include "parsing.hpp"
class Client;
class Server;
class IrcUser;

class CommandHandler
{
    private:
		Server &_server;
		std::map<int, IrcUser> _users;
		void _handlePass(Client &client, const struct Message &msg);
		void _handleNick(Client &client, const struct Message &msg);
		void _handleUser(Client &client, const struct Message &msg);
		void _checkRegistration(Client &client);

    public:
        // CommandHandler();
        CommandHandler(const CommandHandler& other);
        CommandHandler& operator=(const CommandHandler& other);
        ~CommandHandler();

		CommandHandler(Server &server);
		void execute(Client &client, const std::string &line);
		void removeClient(int fd);
};

#endif
