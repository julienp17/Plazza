/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include "Reception.hpp"

void plz::Reception::placeOrder() {
    VecStr_t commands;
    VecStr_t pizza_tokens;
    std::string cmdLine;

    while (std::cin) {
        getline(std::cin, cmdLine);
        commands = split(cmdLine, ';');
        for (auto command : commands) {
            std::cout << "---" << std::endl;
            pizza_tokens = split(command, ' ');
            for (auto token : pizza_tokens)
                std::cout << token << std::endl;
        }
    }
}