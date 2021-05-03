/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** utils
*/

#include "utils.hpp"

VecStr_t split(const std::string &str, char delim) {
    VecStr_t tokens;
    std::istringstream iss(str);
    std::string item;

    while (std::getline(iss, item, delim))
        if (!item.empty())
            tokens.push_back(item);
    return tokens;
}

std::chrono::milliseconds getElapsedTime(
    std::chrono::time_point<std::chrono::steady_clock> t1) {
    auto t2 = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
}
