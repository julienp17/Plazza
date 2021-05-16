/*
** EPITECH PROJECT, 2021
** HorrorRPG [WSL: Ubuntu-20.04]
** File description:
** ReceptionState
*/

#include <iostream>
#include "ReceptionState.hpp"
#include "KitchenState.hpp"
#include "Logger.hpp"

namespace plz {
/* ----------------------------- Ctors / Dtors ------------------------------ */

ReceptionState::ReceptionState(const struct StateData &stateData)
            : AState(stateData) {
    sf::Vector2u winSize = stateData.window->getSize();

    initAnnounceText(winSize);
    initPizzaButtons(winSize);
    initSizeButtons(winSize);
    initCounter(winSize);
    initSendButton(winSize);
    initBackButton(winSize);
    initKitchenButton(winSize);
    _textures["background"].loadFromFile(
        "./assets/reception_background.png");
    _background.setTexture(&_textures["background"]);
    _background.setSize(static_cast<sf::Vector2f>(winSize));
    FILE_LOG(linfo) << "Restaurant is open.";
    FILE_LOG(linfo) << _reception->getKitchenSettings();
    std::cout << "Welcome to Plazza! The Pizzeria for everyone!" << std::endl;
}

/* ------------------------------ Initializers ------------------------------ */

void ReceptionState::initAnnounceText(const sf::Vector2u &winSize) {
    sf::Vector2f pos;

    pos.x = PERCENTAGE(2, winSize.x);
    pos.y = PERCENTAGE(3, winSize.y);
    _announceText.setPosition(pos);
    _announceText.setFont(*_font);
    _announceText.setString("Welcome to Plazza! The Pizzeria for everyone!");
}

void ReceptionState::initPizzaButtons(const sf::Vector2u &winSize) {
    sf::FloatRect rect;
    std::vector<std::string> pizzaButtonsNames = {
        "Margarita", "Regina", "Americana", "Fantasia"
    };
    std::string path;

    rect.left = PERCENTAGE(2, winSize.x);
    rect.top = PERCENTAGE(18, winSize.y);
    rect.width = PERCENTAGE(10, winSize.x);
    rect.height = PERCENTAGE(20, winSize.y);
    for (const std::string &name : pizzaButtonsNames) {
        path = "./assets/" + name + ".png";
        _pizzaButtons[name] = std::make_shared<Button>(
            rect, name, _font, path);
        rect.left += PERCENTAGE(20, winSize.y);
    }
    _selectedPizza = nullptr;
}

void ReceptionState::initSizeButtons(const sf::Vector2u &winSize) {
    sf::FloatRect rect;
    std::vector<std::string> sizeNames = {
        "S", "M", "L", "XL", "XXL"
    };

    rect.left = PERCENTAGE(2, winSize.x);
    rect.top = PERCENTAGE(43, winSize.y);
    rect.width = PERCENTAGE(5, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    for (const std::string &name : sizeNames) {
        _sizeButtons[name] = std::make_shared<Button>(rect, name, _font);
        rect.left += PERCENTAGE(15, winSize.y);
    }
    _selectedSize = nullptr;
}

void ReceptionState::initCounter(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(2, winSize.x);
    rect.top = PERCENTAGE(60, winSize.y);
    rect.width = PERCENTAGE(5, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _counter = std::make_unique<Counter<int>>(rect, _font);
    _counter->setCount(1);
}

void ReceptionState::initSendButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(25, winSize.x);
    rect.top = PERCENTAGE(60, winSize.y);
    rect.width = PERCENTAGE(15, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _sendButton = std::make_unique<Button>(rect, "Send", _font);
}

void ReceptionState::initBackButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(2, winSize.x);
    rect.top = PERCENTAGE(88, winSize.y);
    rect.width = PERCENTAGE(25, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _backButton = std::make_unique<Button>(rect, "<- Go to menu", _font);
}

void ReceptionState::initKitchenButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(73, winSize.x);
    rect.top = PERCENTAGE(88, winSize.y);
    rect.width = PERCENTAGE(25, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _kitchenButton = std::make_unique<Button>(rect, "Go to kitchen ->", _font);
}

/* ---------------------------- Update functions ---------------------------- */

void ReceptionState::update(const millis_t dt) {
    (void)dt;
    _reception->handleDisconnections();
    updateAnnouncement();
    updateSelectedPizza();
    updateSelectedSize();
    updateCounter();
    updateSendButton();
    if (_reception->getNbKitchens() > 0) {
        updateKitchenButton();
    }
    updateBackButton();
}

void ReceptionState::updateAnnouncement(void) {
    std::string received = _reception->handleReceived();

    if (!received.empty()) {
        _announceText.setString("Order " + received + " delivered.");
    }
}

void ReceptionState::updateSelectedPizza(void) {
    for (auto & [_, button] : _pizzaButtons) {
        button->update(sf::Mouse::getPosition(*_win));
        if (button->isPressed()) {
            _selectedPizza = button;
        }
    }
    if (_selectedPizza) {
        _selectedPizza->setColor(sf::Color::Green);
    }
}

void ReceptionState::updateSelectedSize(void) {
    for (auto & [_, button] : _sizeButtons) {
        button->update(sf::Mouse::getPosition(*_win));
        if (button->isPressed()) {
            _selectedSize = button;
        }
    }
    if (_selectedSize) {
        _selectedSize->setColor(sf::Color::Green);
    }
}

void ReceptionState::updateCounter(void) {
    _counter->update(sf::Mouse::getPosition(*_win));
    if (_counter->getCount() < 1) {
        _counter->setCount(1);
    }
}

void ReceptionState::updateSendButton(void) {
    _sendButton->update(sf::Mouse::getPosition(*_win));
    auto shouldSend = [this] (void) {
        return (
            _sendButton->isPressed()
            && _selectedPizza != nullptr
            && _selectedSize != nullptr);
    };
    std::stringstream order;

    if (!shouldSend())
        return;
    order << _selectedPizza->getString() << " " << _selectedSize->getString()
        << " x" << _counter->getCount();
    FILE_LOG(linfo) << "Received command : [" << order.str() << "]";
    if (_reception->placeOrder(order.str())) {
        _announceText.setString("Order placed for " + order.str());
    } else {
        _announceText.setString("Failed to place order for " + order.str());
    }
}

void ReceptionState::updateBackButton(void) {
    _backButton->update(sf::Mouse::getPosition(*_win));
    if (_backButton->isPressed()) {
        _win->clear();
        _announceText.setString("Closing the kitchen, please wait...");
        render();
        _win->display();
        _reception->close();
        _states->pop();
    }
}

void ReceptionState::updateKitchenButton(void) {
    _kitchenButton->update(sf::Mouse::getPosition(*_win));
    if (_kitchenButton->isPressed()) {
        auto pids = _reception->getKitchenPIDs();
        _states->push(std::make_shared<KitchenState>(_stateData, pids));
    }
}

/* ---------------------------- Member functions ---------------------------- */

void ReceptionState::render(void) {
    _win->draw(_background);
    _win->draw(_announceText);
    for (auto & [_, button] : _pizzaButtons)
        button->render(_win);
    for (auto & [_, button] : _sizeButtons)
        button->render(_win);
    _counter->render(_win);
    _sendButton->render(_win);
    _backButton->render(_win);
    if (_reception->getNbKitchens() > 0) {
        _kitchenButton->render(_win);
    }
}
}  // namespace plz
