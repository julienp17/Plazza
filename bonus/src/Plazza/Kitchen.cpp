/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Kitchen
*/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include "plazza.hpp"
#include "Kitchen.hpp"
#include "Error.hpp"
#include "Logger.hpp"

namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */

Kitchen::Kitchen(void) {
    this->init();
    this->_msgQueue = nullptr;
}

Kitchen::Kitchen(const KitchenSettings &settings,
                std::shared_ptr<MessageQueue> msgQ) {
    this->_settings = settings;
    this->init();
    this->_msgQueue = msgQ;
    this->_msgQueue->send(CONNECTION, "Connected");
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
    std::vector<std::string> names = getRandomNames();

    this->_cooks.clear();
    this->_cooks.reserve(this->_settings.nbCooks);
    for (size_t id = 1 ; id < _settings.nbCooks + 1 ; id++) {
        cook = std::make_shared<Cook>(Cook(id, names[rand() % names.size()]));
        _cooks.push_back(std::make_pair(cook, std::thread()));
    }
}

/* ---------------------------- Member functions ---------------------------- */

void Kitchen::run(void) {
    this->_isOpen = true;
    this->putCooksToWork();
    while (this->_isOpen) {
        this->handleReceived();
        if (this->shouldRestock()) {
            this->restock();
            resetTimepoint(this->_lastRestock);
        }
        if (this->isActive())
            resetTimepoint(this->_lastActive);
        if (this->shouldClose())
            this->_isOpen = false;
    }
    {
        std::lock_guard<std::mutex> lk(_msgQueueMutex);
        if (this->_msgQueue != nullptr)
            _msgQueue->send(DISCONNECTION, "Disconnection");
        FILE_LOG(linfo) << "Kitchen " << getpid() << " closed.";
    }
}

void Kitchen::cookWorker(std::shared_ptr<Cook> cook) {
    std::stringstream pizzaStr;
    std::shared_ptr<Pizza> pizza = nullptr;

    while (this->_isOpen) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        if (!_pizzaQueue.empty() && this->canMakePizza(_pizzaQueue.front())) {
            cook->setPizza(_pizzaQueue.front());
            _pizzaQueue.pop();
            lock.unlock();
            pizza = cook->getPizza();
            FILE_LOG(linfo) << cook->getName() << " is handling " << *pizza;
            this->useIngredients(pizza->ingredients);
            cook->makePizza(_settings.cookingMultiplier);
            FILE_LOG(linfo) << cook->getName() << " finished making " << *pizza;
            pizzaStr.str("");
            pizzaStr.clear();
            pizzaStr << *pizza;
            {
                std::lock_guard<std::mutex> lk(_msgQueueMutex);
                if (this->_msgQueue != nullptr)
                    this->_msgQueue->send(PIZZA, pizzaStr.str());
                FILE_LOG(linfo) << cook->getName() << " sent " << *pizza;
                std::this_thread::sleep_for(10ms);
            }
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
    resetTimepoint(this->_lastActive);
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

void Kitchen::handleReceived(void) {
    std::lock_guard<std::mutex> lk(_msgQueueMutex);
    std::string message;

    if (this->_msgQueue == nullptr)
        return;
    message = _msgQueue->recv(ASK_ORDER, MSG_NOERROR | IPC_NOWAIT);
    if (!message.empty())
        this->respondOrder(message);
    message = _msgQueue->recv(ASK_STATUS, MSG_NOERROR | IPC_NOWAIT);
    if (!message.empty())
        this->respondStatus(message);
}

void Kitchen::respondOrder(const std::string &message) {
    std::vector<std::string> tokens = split(message, ' ');
    auto pizza = std::make_shared<Pizza>(tokens[0], tokens[1], tokens[2]);

    if (this->canAddPizzas(1)) {
        _msgQueue->send(ORDER, "Accepted");
        this->addPizza(pizza);
    } else {
        _msgQueue->send(ORDER, "Refused");
    }
}

void Kitchen::respondStatus(const std::string &message) {
    std::stringstream stream;

    (void)message;
    stream << *this;
    _msgQueue->send(STATUS, stream.str());
}

/* ---------------------------- Getters / Setters --------------------------- */

bool Kitchen::shouldClose(void) const {
    return getElapsedTime(this->_lastActive) > _settings.inactiveTime;
}

bool Kitchen::shouldRestock(void) const {
    return getElapsedTime(this->_lastRestock) > _settings.restockTime;
}

bool Kitchen::isActive(void) const {
    for (auto &pair : _cooks)
        if (pair.first->isWorking())
            return true;
    return _pizzaQueue.empty() == false;
}

bool Kitchen::canAddPizzas(const size_t nbPizzas) {
    return (
        this->getNbPizzasProcessed() + nbPizzas
        <= 2 * this->_settings.nbCooks);
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

const std::vector<std::shared_ptr<Cook>> Kitchen::getCooks(void) const {
    std::vector<std::shared_ptr<Cook>> cooks;

    for (auto &pair : _cooks)
        cooks.push_back(pair.first);
    return cooks;
}

size_t Kitchen::getNbPizzasProcessed(void)  {
    size_t processed = 0;

    for (const auto &pair : _cooks)
        if (pair.first->isWorking())
            processed++;
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        processed += _pizzaQueue.size();
    }
    return processed;
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
    auto queue = kitchen.getQueue();

    if (queue.empty()) {
        out << "None!" << std::endl;
    } else {
        while (!queue.empty()) {
            out << *queue.front();
            queue.pop();
            if (!queue.empty()) {
                out << ";";
            }
        }
        out << std::endl;
    }
    for (const auto &cook : cooks) {
        out << *cook << std::endl;
    }
    for (const auto &[name, remaining] : stock) {
        out << name << ":" << remaining << std::endl;
    }
    return out;
}
