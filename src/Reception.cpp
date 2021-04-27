/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include "Reception.hpp"
#include "Error.hpp"
#include "utils.hpp"

namespace plz {
void Reception::run(void) {
    while (std::cin)
        this->placeOrders();
}

void Reception::placeOrders(void) {
    VecStr_t orders;
    std::string cmdLine;

    getline(std::cin, cmdLine);
    orders = split(cmdLine, ';');
    for (const std::string &order : orders)
        this->placeOrder(order);
}

void Reception::placeOrder(const std::string &order) {
    PizzaType type;
    PizzaSize size;
    size_t nbPizzas = 0;
    VecStr_t tokens = split(order, ' ');

    if (!this->orderIsCorrect(tokens)) {
        std::cout << "Invalid order." << std::endl;
        return;
    }
    type = getPizzaType(tokens[0]);
    size = getPizzaSize(tokens[1]);
    nbPizzas = getNumber<size_t>(tokens[2]);
    for (size_t i = 0 ; i < nbPizzas ; i++)
        _pizzas.push_back(Pizza(type, size));
}

bool Reception::orderIsCorrect(const VecStr_t &tokens) {
    if (tokens.size() != 3) {
        std::cerr << "Pizza ordering must be: TYPE SIZE NUMBER" << std::endl;
        return false;
    }
    try {
        getPizzaType(tokens[0]);
        getPizzaSize(tokens[1]);
    } catch (const PizzaError &err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    if (!isPositiveNumber<size_t>(tokens[2])) {
        std::cerr << "Invalid order." << std::endl;
        return false;
    }
    return true;
}
}  // namespace plz
