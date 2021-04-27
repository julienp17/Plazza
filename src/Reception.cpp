/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include "Reception.hpp"

void plz::Reception::placeOrders(void) {
    VecStr_t orders;
    std::string cmdLine;

    while (std::cin) {
        getline(std::cin, cmdLine);
        orders = split(cmdLine, ';');
        for (const std::string &order : orders)
            this->placeOrder(order);
    }
}

void plz::Reception::placeOrder(const std::string &order) {
    VecStr_t pizza_tokens;

    std::cout << "---" << std::endl;
    pizza_tokens = split(order, ' ');
    for (auto token : pizza_tokens)
        std::cout << token << std::endl;
}
