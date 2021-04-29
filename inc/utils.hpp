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
#include <chrono>

using VecStr_t = std::vector<std::string>;
using millis_t = size_t;

VecStr_t split(const std::string &str, char delim);
millis_t getElapsedTime(std::chrono::time_point<std::chrono::steady_clock> t1);

template <typename T>
bool isPositiveNumber(const std::string &str) {
    std::istringstream iss(str);
    T nb;

    if (str[0] == '-')
        return false;
    iss >> nb;
    return iss.eof() && !iss.fail();
}

template <typename T>
T getNumber(const std::string &str) {
    std::istringstream iss(str);
    T nb;

    iss >> nb;
    return nb;
}

#endif  // INC_UTILS_HPP_
