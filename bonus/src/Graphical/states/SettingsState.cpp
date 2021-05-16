/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** SettingsState
*/

#include "SettingsState.hpp"

namespace plz {
/* ----------------------------- Ctors / Dtors ------------------------------ */

SettingsState::SettingsState(const struct StateData &stateData)
            : AState(stateData) {
    sf::FloatRect rect;
    sf::Vector2u winSize = stateData.window->getSize();

    _settings = this->_reception->getKitchenSettings();
    this->_textures["background"].loadFromFile("./assets/menu_background.png");
    this->initBackButton(winSize);
    this->initCookingMultiplier(winSize);
    this->initNbCooks(winSize);
    this->initRestockTime(winSize);
}

/* ------------------------------ Initializers ------------------------------ */

void SettingsState::initBackButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(5, winSize.x);
    rect.top = PERCENTAGE(5, winSize.y);
    rect.width = PERCENTAGE(15, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    this->_backButton = std::make_unique<Button>(rect, "Back", _font);
}

void SettingsState::initCookingMultiplier(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(30, winSize.x);
    rect.top = PERCENTAGE(40, winSize.y);
    rect.width = PERCENTAGE(10, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _cookingMultiplier = std::make_unique<Counter<float>>(
        rect, _font, "Cooking multiplier (less is faster)");
    _cookingMultiplier->setCount(_settings.cookingMultiplier);
}

void SettingsState::initNbCooks(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(30, winSize.x);
    rect.top = PERCENTAGE(60, winSize.y);
    rect.width = PERCENTAGE(10, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _nbCooks = std::make_unique<Counter<int>>(rect, _font, "Number of cooks");
    _nbCooks->setCount(_settings.nbCooks);
}

void SettingsState::initRestockTime(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(30, winSize.x);
    rect.top = PERCENTAGE(80, winSize.y);
    rect.width = PERCENTAGE(10, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    this->_restockTime = std::make_unique<Counter<int>>(
        rect, _font, "Restocking time (in seconds)");
    this->_restockTime->setCount(_settings.restockTime.count() / 1000);
}

/* ---------------------------- Member functions ---------------------------- */

void SettingsState::update(const millis_t dt) {
    (void)dt;
    updateCookingMultiplier();
    updateNbCooks();
    updateRestockTime();
    updateBackButton();
}

void SettingsState::updateCookingMultiplier(void) {
    _cookingMultiplier->update(sf::Mouse::getPosition(*_win));
    if (_cookingMultiplier->getCount() <= 0) {
        _cookingMultiplier->setCount(0.1f);
    }
    if (_cookingMultiplier->getCount() <= 1.0f) {
        _cookingMultiplier->setStep(0.1f);
    } else if (_cookingMultiplier->getCount() == 1.1f) {
        _cookingMultiplier->setCount(2);
        _cookingMultiplier->setStep(1);
    }
}

void SettingsState::updateNbCooks(void) {
    _nbCooks->update(sf::Mouse::getPosition(*_win));
    if (_nbCooks->getCount() < 2) {
        _nbCooks->setCount(2);
    } else if (_nbCooks->getCount() > 6) {
        _nbCooks->setCount(6);
    }
}

void SettingsState::updateRestockTime(void) {
    _restockTime->update(sf::Mouse::getPosition(*_win));
    if (_restockTime->getCount() < 1) {
        _restockTime->setCount(1);
    }
}

void SettingsState::updateBackButton(void) {
    _backButton->update(sf::Mouse::getPosition(*_win));
    if (_backButton->isPressed()) {
        _settings.cookingMultiplier = _cookingMultiplier->getCount();
        _settings.nbCooks = _nbCooks->getCount();
        _settings.restockTime = std::chrono::milliseconds(
            _restockTime->getCount() * 1000);
        _reception->setKitchenSettings(_settings);
        _states->pop();
    }
}

void SettingsState::render(void) {
    sf::RectangleShape background;

    background.setTexture(&this->_textures["background"]);
    background.setSize(static_cast<sf::Vector2f>(_win->getSize()));
    _win->draw(background);
    this->_backButton->render(_win);
    this->_cookingMultiplier->render(_win);
    this->_nbCooks->render(_win);
    this->_restockTime->render(_win);
}
}  // namespace plz
