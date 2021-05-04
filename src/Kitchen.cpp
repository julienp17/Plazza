/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#include <iostream>
#include "plazza.hpp"
#include "Kitchen.hpp"
#include "Error.hpp"

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

Kitchen::~Kitchen(void) {
    for (auto &cook : _cooks)
        if (cook.joinable())
            cook.join();
}

/* ------------------------ Initialization functions ------------------------ */

void Kitchen::init(void) {
    this->_isOpen = false;
    this->initStock();
    this->_cooks.clear();
    this->_cooks.reserve(this->_settings.nbCooks);
    resetTimepoint(this->_restockTimepoint);
    resetTimepoint(this->_activeTimepoint);
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

void Kitchen::run(void) {
    this->_isOpen = true;
    this->putCooksToWork();
    resetTimepoint(this->_restockTimepoint);
    resetTimepoint(this->_activeTimepoint);
    this->addPizza(std::make_shared<Pizza>(Margarita, S));
    while (this->_isOpen) {
        if (this->shouldRestock()) {
            std::cout << "Time to restock !" << std::endl;
            this->restock();
            this->addPizza(std::make_shared<Pizza>(Margarita, L));
            resetTimepoint(this->_restockTimepoint);
        }
        if (this->shouldClose()) {
            std::cout << "Time to close!" << std::endl;
            this->_isOpen = false;
        }
    }
}

void Kitchen::putCooksToWork(void) {
    std::string name;

    try {
        for (size_t i = 0 ; i < _settings.nbCooks ; i++) {
            name = getRandomName();
            _cooks.push_back(std::thread(&Kitchen::cookWorker, this, name));
        }
    } catch (const std::system_error &err) {
        throw KitchenError(err.what());
    }
}

void Kitchen::cookWorker(const std::string &name) {
    std::shared_ptr<Pizza> pizza = nullptr;

    // std::cout << "Hey i'm starting to work" << std::endl;
    while (this->_isOpen) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        if (!_pizzas.empty()) {
            pizza = _pizzas.front();
            _pizzas.pop();
            lock.unlock();
            std::cout << name << ": Hey i'm making the " << *pizza << std::endl;
            std::this_thread::sleep_for(pizza->timeToBake * _settings.cookingMultiplier);
            std::cout << name << ": Hey I finished the " << *pizza << std::endl;
        } else {
            lock.unlock();
            std::this_thread::yield();
        }
    }
}

bool Kitchen::shouldClose(void) const {
    return getElapsedTime(this->_activeTimepoint) > _settings.inactiveTime;
}

bool Kitchen::shouldRestock(void) const {
    return getElapsedTime(this->_restockTimepoint) > _settings.restockTime;
}

void Kitchen::restock(void) {
    for (auto & [ name, remaining ] : this->_stock)
        remaining += _settings.restockNb;
}

void Kitchen::useIngredient(const std::string &name) {
    if (_stock.find(name) == _stock.end())
        throw KitchenError("Cannot decrement a stock to -1.");
    if (_stock[name] > 0)
        _stock[name]--;
}
}  // namespace plz

/* -------------------------- Operator overloading -------------------------- */

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen) {
    auto stock = kitchen.getStock();

    for (const auto & [ name, remaining ] : stock)
        out << "    " << name << ": " << remaining << std::endl;
    return out;
}
