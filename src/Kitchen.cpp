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
/* ----------------------- Constructors / Destructors ----------------------- */

KitchenSettings::KitchenSettings(void) {
    this->cookingMultiplier = 1.0f;
    this->nbCooks = 5;
    this->startNbIngredients = 5;
    this->inactiveTime = std::chrono::milliseconds(5000);
    this->restockTime = std::chrono::milliseconds(2000);
    this->restockNb = 1;
}

Kitchen::Kitchen(void) {
    this->init();
}

Kitchen::Kitchen(const KitchenSettings &settings) {
    this->_settings = settings;
    this->init();
}

/* ------------------------ Initialization functions ------------------------ */

void Kitchen::init(void) {
    this->initStock();
    this->_restockTimepoint = std::chrono::steady_clock::now();
    this->_activeTimepoint = std::chrono::steady_clock::now();
}

void Kitchen::initStock(void) {
    VecStr_t ingredientsName = {
        "doe", "tomato", "gruyere", "ham", "mushrooms", "steak", "eggplant",
        "goat cheese", "chief love"
    };

    for (const std::string &name : ingredientsName)
        this->_stock[name] = this->_settings.startNbIngredients;
}

/* ---------------------------- Member functions ---------------------------- */

void Kitchen::loop(void) {
    std::chrono::milliseconds elapsedTime(0);

    while (elapsedTime < _settings.inactiveTime) {
        elapsedTime = getElapsedTime(this->_restockTimepoint);
        if (elapsedTime > _settings.restockTime) {
            std::cout << "Time to restock !" << std::endl;
            this->restock();
            this->_restockTimepoint = std::chrono::steady_clock::now();
        }
        elapsedTime = getElapsedTime(this->_activeTimepoint);
    }
    std::cout << "Time to close!" << std::endl;
}

void Kitchen::restock(void) {
    for (auto & [ name, remaining ] : this->_stock)
        remaining += _settings.restockNb;
}

}  // namespace plz

/* -------------------------- Operator overloading -------------------------- */

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen) {
    auto stock = kitchen.getStock();

    for (const auto & [ name, remaining ] : stock)
        out << "    " << name << ": " << remaining << std::endl;
    return out;
}
