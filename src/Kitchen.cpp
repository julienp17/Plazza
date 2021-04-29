/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#include <iostream>
#include "plazza.hpp"
#include "Kitchen.hpp"

namespace plz {
Kitchen::Kitchen(void) {
    this->initStock(5);
    _restockTimepoint = std::chrono::steady_clock::now();
    _activeTimepoint = std::chrono::steady_clock::now();
}

void Kitchen::loop(void) {
    millis_t elapsedTime = 0;

    while (elapsedTime < 5000) {
        elapsedTime = getElapsedTime(_restockTimepoint);
        if (elapsedTime > 2000) {
            std::cout << "here" << std::endl;
            _restockTimepoint = std::chrono::steady_clock::now();
        }
        elapsedTime = getElapsedTime(_activeTimepoint);
    }
    std::cout << "im dead" << std::endl;
}

void Kitchen::initStock(const size_t startNb) {
    VecStr_t ingredientsName = {
        "doe", "tomato", "gruyere", "ham", "mushrooms", "steak", "eggplant",
        "goat cheese", "chief love"
    };

    for (const std::string &name : ingredientsName)
        _stock[name] = startNb;
}
}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen) {
    auto stock = kitchen.getStock();

    for (const auto & [ name, remaining ] : stock)
        out << "    " << name << ": " << remaining << std::endl;
    return out;
}