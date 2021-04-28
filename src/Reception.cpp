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
Reception::Reception(const float cookingMultiplier, const size_t nbCooks,
                    const milliseconds_t stockTime) {
    this->_cookingMultiplier = cookingMultiplier;
    this->_nbCooks = nbCooks;
    this->_stockTime = stockTime;
}

void Reception::placeOrders(const std::string &orders_str) {
    VecStr_t orders;

    orders = split(orders_str, ';');
    for (const std::string &order : orders)
        this->placeOrder(order);
}

bool Reception::placeOrder(const std::string &order) {
    PizzaType type;
    PizzaSize size;
    size_t nbPizzas = 0;
    VecStr_t tokens = split(order, ' ');

    if (!this->orderIsCorrect(tokens)) {
        std::cout << "Invalid order." << std::endl;
        return false;
    }
    type = getPizzaType(tokens[0]);
    size = getPizzaSize(tokens[1]);
    nbPizzas = getNumber<size_t>(tokens[2].substr(1));
    for (size_t i = 0 ; i < nbPizzas ; i++)
        _pizzas.push(PizzaPtr(new Pizza(type, size)));
    std::cout << "Order placed for " << nbPizzas << " "
        << tokens[0] << " " << tokens[1] << "." << std::endl;
    return true;
}

bool Reception::orderIsCorrect(const VecStr_t &tokens) {
    const char *nbPizzas = NULL;

    if (tokens.size() != 3) {
        std::cerr << "Pizza ordering must be: TYPE SIZE NUMBER." << std::endl;
        return false;
    }
    try {
        getPizzaType(tokens[0]);
        getPizzaSize(tokens[1]);
    } catch (const PizzaError &err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    nbPizzas = tokens[2].c_str() + 1;
    if (!isPositiveNumber<size_t>(nbPizzas) || !getNumber<size_t>(nbPizzas)) {
        std::cerr << "The number of pizzas must be positive." << std::endl;
        return false;
    }
    return true;
}
}  // namespace plz
