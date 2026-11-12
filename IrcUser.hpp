/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbastiat <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:39:39 by jbastiat          #+#    #+#             */
/*   Updated: 2026/11/12 15:43:30 by jbastiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCUSER_HPP
#define IRCUSER_HPP

# include <string>

class IrcUser
{
    private:

    public:
        IrcUser();
        IrcUser(const IrcUser& other);
        IrcUser& operator=(const IrcUser& other);
        ~IrcUser();
		std::string nickname;
		std::string username;
		std::string realname;

		bool passwordAccepted;
		bool hasNickname;
		bool hasUsername;
		bool registered;
};

#endif
