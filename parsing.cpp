/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbastiat <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 10:27:20 by jbastiat          #+#    #+#             */
/*   Updated: 2026/11/06 11:36:09 by jbastiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <string>
#include <vector>

int parsing(std::string cmd)
{
	unsigned long pos = 0;
	unsigned long end;
	unsigned long paramCount = 0;
	std::string prefix;
	std::string command;
	std::vector<std::string> parameters;
	std::string trailing;
	bool hasTrailing = false;

    pos = cmd.find_first_not_of(' ');
	if (pos == std::string::npos)
        return (1);
    if (cmd[pos] == ':'){
        end = cmd.find(' ', pos);
        if (end == std::string::npos)
            return (1);
        prefix = cmd.substr(pos + 1, end - pos - 1);
        pos = cmd.find_first_not_of(' ', end);
    }
	if (pos == std::string::npos)
        return (1);
	    end = cmd.find(' ', pos);
    if (end == std::string::npos)
        end = cmd.size();
    command = cmd.substr(pos, end - pos);
    pos = end;
	while (pos < cmd.size()){
        pos = cmd.find_first_not_of(' ', pos);
        if (pos == std::string::npos)
            break;
        if (cmd[pos] == ':')
        {
			hasTrailing = true;
            trailing = cmd.substr(pos + 1);
			if (paramCount >= 15)
				return (1);
			paramCount++;
            break;
        }
        end = cmd.find(' ', pos);
        if (end == std::string::npos)
            end = cmd.size();
		if (paramCount >= 15)
    		return (1);
        parameters.push_back(cmd.substr(pos, end - pos));
		paramCount++;
        pos = end;
    }
    std::cout << "Prefix: " << prefix << std::endl;
    std::cout << "Command: " << command << std::endl;
    for (unsigned long i = 0; i < parameters.size(); i++)
        std::cout << "Parameter: " << parameters[i] << std::endl;
    std::cout << "Trailing: " << trailing << std::endl;
	return (0);
}

// 
// 
// int main(int ac, char **av)
// {
	// if (ac < 2)
		// return (1);
	// parsing(":nick!user@host PRIVMSG #test :Hello world");
	// (void)av;
	// return (0);
// }
