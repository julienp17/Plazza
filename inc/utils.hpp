/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** utils
*/

#ifndef INC_UTILS_HPP_
#define INC_UTILS_HPP_

#include <string>
#include <sstream>
#include <vector>

typedef std::vector<std::string> VecStr_t;

VecStr_t split(const std::string &str, char delim);

template <typename T>
bool isPositiveNumber(const std::string &str) {
    std::istringstream iss(str);
    T nb;

    if (str[0] == '-')
        return false;
    iss >> nb;
    return iss.eof() && !iss.fail();
}

#endif  // INC_UTILS_HPP_
