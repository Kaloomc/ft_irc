/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbastiat <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 15:14:17 by jbastiat          #+#    #+#             */
/*   Updated: 2026/11/12 15:17:35 by jbastiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSING_HPP
#define PARSING_HPP
#include <iostream>
#include <string>
#include <vector>

struct Message
{
    std::string prefix;
    std::string command;
    std::vector<std::string> parameters;
    std::string trailing;
    bool hasPrefix;
    bool hasTrailing;
	bool valid;
};

Message parsing(std::string cmd);

#endif
