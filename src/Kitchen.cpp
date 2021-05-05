/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#include <iostream>
#include <algorithm>
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
    for (auto &pair : _cooks) {
        if (pair.second.joinable())
            pair.second.join();
    }
}

/* ------------------------ Initialization functions ------------------------ */

void Kitchen::init(void) {
    this->_isOpen = false;
    this->initStock();
    this->initCooks();
    resetTimepoint(this->_lastRestock);
    resetTimepoint(this->_lastActive);
}

void Kitchen::initStock(void) {
    VecStr_t ingredientsName = {
        "doe", "tomato", "gruyere", "ham", "mushrooms", "steak", "eggplant",
        "goat cheese", "chief love"
    };

    for (const std::string &name : ingredientsName)
        this->_stock[name] = this->_settings.startNbIngredients;
}

void Kitchen::initCooks(void) {
    std::shared_ptr<Cook> cook = nullptr;

    this->_cooks.clear();
    this->_cooks.reserve(this->_settings.nbCooks);
    for (size_t id = 1 ; id < _settings.nbCooks + 1 ; id++) {
        cook = std::make_shared<Cook>(Cook(id, getRandomName()));
        _cooks.push_back(std::make_pair(cook, std::thread()));
    }
}

/* ---------------------------- Member functions ---------------------------- */

void Kitchen::run(void) {
    this->_isOpen = true;
    this->putCooksToWork();
    resetTimepoint(this->_lastRestock);
    resetTimepoint(this->_lastActive);
    while (this->_isOpen) {
        if (this->shouldRestock()) {
            this->addPizza(std::make_shared<Pizza>(Regina, S));
            this->addPizza(std::make_shared<Pizza>(Margarita, M));
            this->addPizza(std::make_shared<Pizza>(Americana, XL));
            this->addPizza(std::make_shared<Pizza>(Fantasia, XXL));
            this->restock();
            resetTimepoint(this->_lastRestock);
        }
        if (this->shouldClose()) {
            std::cout << "Time to close!" << std::endl;
            this->_isOpen = false;
        }
    }
}

std::shared_ptr<Cook> Kitchen::getCook(const size_t id) {
    for (auto &cook : _cooks)
        if (cook.first->getID() == id)
            return cook.first;
    throw KitchenError("No cook with the id " + std::to_string(id));
}

void Kitchen::putCooksToWork(void) {
    size_t id = 0;

    try {
        for (auto &cook : _cooks) {
            id = cook.first->getID();
            cook.second = std::thread(&Kitchen::cookWorker, this, id);
        }
    } catch (const std::system_error &err) {
        throw KitchenError(err.what());
    }
}

void Kitchen::cookWorker(const size_t id) {
    // TODO(julien): put cook directly in the argument ?
    std::shared_ptr<Cook> cook = this->getCook(id);

    while (this->_isOpen) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        if (!_pizzaQueue.empty()) {
            cook->setPizza(_pizzaQueue.front());
            _pizzaQueue.pop();
            lock.unlock();
            cook->makePizza(_settings.cookingMultiplier);
        } else {
            lock.unlock();
            std::this_thread::yield();
        }
    }
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
/* ---------------------------- Getters / Setters --------------------------- */

bool Kitchen::shouldClose(void) const {
    return getElapsedTime(this->_lastActive) > _settings.inactiveTime;
}

bool Kitchen::shouldRestock(void) const {
    return getElapsedTime(this->_lastRestock) > _settings.restockTime;
}

void Kitchen::setSettings(const KitchenSettings &settings) {
    bool resetCooks = (_settings.nbCooks != settings.nbCooks);

    _settings = settings;
    if (resetCooks)
        this->initCooks();
}

}  // namespace plz

/* -------------------------- Operator overloading -------------------------- */

std::ostream &operator<<(std::ostream &out, const plz::Kitchen &kitchen) {
    auto stock = kitchen.getStock();

    for (const auto & [ name, remaining ] : stock)
        out << "    " << name << ": " << remaining << std::endl;
    return out;
}
