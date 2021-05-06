/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "plazza.hpp"
#include "Kitchen.hpp"
#include "Error.hpp"


namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */

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
    while (this->_isOpen) {
        if (this->shouldRestock()) {
            this->restock();
            resetTimepoint(this->_lastRestock);
        }
        if (this->shouldClose()) {
            std::cout << "Time to close!" << std::endl;
            this->_isOpen = false;
        }
    }
}

void Kitchen::cookWorker(std::shared_ptr<Cook> cook) {
    while (this->_isOpen) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        if (!_pizzaQueue.empty() && this->canMakePizza(_pizzaQueue.front())) {
            cook->setPizza(_pizzaQueue.front());
            _pizzaQueue.pop();
            lock.unlock();
            this->useIngredients(cook->getPizza()->ingredients);
            cook->makePizza(_settings.cookingMultiplier);
        } else {
            lock.unlock();
            std::this_thread::yield();
        }
    }
}

void Kitchen::putCooksToWork(void) {
    try {
        for (auto &pair : _cooks)
            pair.second = std::thread(&Kitchen::cookWorker, this, pair.first);
    } catch (const std::system_error &err) {
        throw KitchenError(err.what());
    }
}

void Kitchen::addPizza(std::shared_ptr<Pizza> pizza) {
    std::lock_guard<std::mutex> lock(_queueMutex);

    _pizzaQueue.push(pizza);
}

void Kitchen::restock(void) {
    std::lock_guard<std::mutex> lock(_stockMutex);

    for (auto & [ name, remaining ] : this->_stock)
        remaining += _settings.restockNb;
}

void Kitchen::useIngredients(const std::vector<std::string> &ingredients) {
    std::lock_guard<std::mutex> lock(_stockMutex);

    for (const std::string &name : ingredients) {
        if (_stock.find(name) == _stock.end())
            throw KitchenError("Ingredient " + name + " not found in stock.");
        if (_stock[name] > 0)
            _stock[name]--;
    }
}

/* ---------------------------- Getters / Setters --------------------------- */

bool Kitchen::shouldClose(void) const {
    return getElapsedTime(this->_lastActive) > _settings.inactiveTime;
}

bool Kitchen::shouldRestock(void) const {
    return getElapsedTime(this->_lastRestock) > _settings.restockTime;
}

size_t Kitchen::getNbAvailableCooks(void) const {
    size_t nbAvailableCooks = 0;

    for (auto &pair : _cooks)
        if (!pair.first->isWorking())
            nbAvailableCooks++;
    return nbAvailableCooks;
}

bool Kitchen::canAddPizzas(const size_t nbPizzas) {
    std::lock_guard<std::mutex> lock(_queueMutex);
    size_t slots = getNbAvailableCooks()
                    + this->_settings.nbCooks - _pizzaQueue.size();

    return nbPizzas <= slots;
}

bool Kitchen::canMakePizza(const std::shared_ptr<Pizza> pizza) {
    std::lock_guard<std::mutex> lock(_stockMutex);

    for (const std::string &name : pizza->ingredients) {
        auto it = _stock.find(name);
        if (it == _stock.end())
            throw KitchenError("Ingredient " + name + " not found in stock.");
        if (it->second == 0)
            return false;
    }
    return true;
}

std::vector<std::shared_ptr<Cook>> Kitchen::getCooks(void) const {
    std::vector<std::shared_ptr<Cook>> cooks;

    for (auto &pair : _cooks)
        cooks.push_back(pair.first);
    return cooks;
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
    auto cooks = kitchen.getCooks();

    out << "Cooks:" << std::endl;
    for (const auto &cook : cooks)
        out << " - " << *cook << std::endl;
    out << "Stock:" << std::endl;
    for (const auto & [ name, remaining ] : stock)
        out << " - " << std::setw(11) << name << ": " << remaining << std::endl;
    return out;
}
