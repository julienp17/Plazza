/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Reception
*/

#include <iostream>
#include <unistd.h>
#include "Reception.hpp"
#include "Error.hpp"
#include "utils.hpp"
#include <sys/wait.h>

namespace plz {

/* ---------------------------- Member functions ---------------------------- */

void Reception::placeOrders(const std::string &orders_str) {
    VecStr_t orders;

    orders = split(orders_str, ';');
    for (const std::string &order : orders)
        this->placeOrder(order);
}

bool Reception::placeOrder(const std::string &order) {
    std::shared_ptr<Pizza> pizza = nullptr;
    size_t nbPizzas = 0;
    VecStr_t tokens = split(order, ' ');

    if (!this->orderIsCorrect(tokens)) {
        std::cout << "Invalid order." << std::endl;
        return false;
    }
    pizza = std::make_shared<Pizza>(getPizzaType(tokens[0]),
                                    getPizzaSize(tokens[1]));
    nbPizzas = getNumber<size_t>(tokens[2].substr(1));
    for (size_t i = 0 ; i < nbPizzas ; i++)
        _pizzas.push(pizza);
    std::cout << "Order placed for " << nbPizzas << " " << *pizza << std::endl;
    this->createKitchen();
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

void Reception::createKitchen(void) {
    MessageQueue msgQueue(MAIN_FILE_PATH, _msgQueues.size());
    std::string message = "Are you here ?";
    pid_t pid = fork();

    if (pid == -1) {
        throw ReceptionError();
    } else if (pid == 0) {
        plz::Kitchen kitchen(this->_kitchenSettings, msgQueue);
        _exit(0);
    }
    _msgQueues[pid] = msgQueue;
    waitpid(pid, NULL, 0);
    std::cout << msgQueue.recv(1, MSG_NOERROR) << std::endl;
}
}  // namespace plz
