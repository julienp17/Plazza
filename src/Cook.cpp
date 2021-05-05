/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Cook
*/

#include "Cook.hpp"

namespace plz {
std::shared_ptr<Pizza> Cook::makePizza(std::shared_ptr<Pizza> pizza,
                                        const float multiplier) {
    _pizza = pizza;
    std::this_thread::sleep_for(pizza->timeToBake * multiplier);
    _pizza = nullptr;
    return pizza;
}
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Cook &cook) {
    out << cook.getName() << ": ";
    if (cook.isWorking())
        out << "Working on " << *cook.getPizza() << ".";
    else
        out << "Available.";
    return out;
}
