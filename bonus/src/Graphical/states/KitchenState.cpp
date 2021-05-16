/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** KitchenState
*/

#include "KitchenState.hpp"

namespace plz {
/* ----------------------------- Ctors / Dtors ------------------------------ */

KitchenState::KitchenState(const struct StateData &stateData,
                            const std::vector<pid_t> &pids)
            : AState(stateData) {
    sf::Vector2u winSize = stateData.window->getSize();
    std::string status;
    std::vector<std::string> lines;

    _pid = pids[0];
    _otherKitchensPids = pids;
    _otherKitchensPids.erase(_otherKitchensPids.begin());
    _timePassed = 0;
    initBackButton(winSize);
    initNextKitchenButton(winSize);
    initAnnounceText(winSize);
    status = _reception->getStatus(_pid);
    if (status.empty()) {
        _announceText.setString("Kitchen closed.");
        _activated = false;
    } else {
        lines = split(status, '\n');
        initNextPizzaText(winSize, lines);
        initCookBanners(winSize, lines);
        initIngredientBanners(winSize, lines);
        _activated = true;
    }
    _textures["background"].loadFromFile("./assets/kitchen_background.png");
    _background.setTexture(&_textures["background"]);
    _background.setSize(static_cast<sf::Vector2f>(winSize));
}

/* ------------------------------ Initializers ------------------------------ */

void KitchenState::initAnnounceText(const sf::Vector2u &winSize) {
    sf::Vector2f pos;
    sf::FloatRect bounds;

    _announceText.setFont(*_font);
    _announceText.setString("Kitchen " + std::to_string(_pid));
    _announceText.setFillColor(sf::Color::White);
    bounds = _announceText.getGlobalBounds();
    pos.x = PERCENTAGE(50, winSize.x) - bounds.width / 2;
    pos.y = PERCENTAGE(5, winSize.y);
    _announceText.setPosition(pos);
}

void KitchenState::initNextPizzaText(const sf::Vector2u &winSize,
        std::vector<std::string> &statusLines) {
    sf::Vector2f pos;
    sf::FloatRect bounds;
    std::vector<std::string> tokens = split(statusLines[0], ';');

    _nextPizzaText.setFont(*_font);
    _nextPizzaText.setString(tokens[0]);
    _nextPizzaText.setFillColor(sf::Color::White);
    bounds = _nextPizzaText.getGlobalBounds();
    pos.x = PERCENTAGE(75, winSize.x) - bounds.width / 2;
    pos.y = PERCENTAGE(30, winSize.y);
    _nextPizzaText.setPosition(pos);
    statusLines.erase(statusLines.begin());
}

void KitchenState::initCookBanners(const sf::Vector2u &winSize,
        std::vector<std::string> &statusLines) {
    sf::FloatRect rect;
    KitchenSettings settings = _reception->getKitchenSettings();
    std::vector<std::string> tokens;
    int id = 0;
    std::string name;
    std::string activity;

    rect.left = 0;
    rect.top = PERCENTAGE(75, winSize.y);
    rect.width = PERCENTAGE(40, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _textures["cookFace"].loadFromFile("./assets/unknown_face.png");
    for (size_t i = 0 ; i < settings.nbCooks ; i++) {
        tokens = split(statusLines[0], ':');
        id = atoi(tokens[0].c_str());
        name = tokens[1];
        activity = tokens[2];
        _cookBanners[id] = std::make_unique<Banner>(
            rect, _textures["cookFace"], name, activity, _font);
        statusLines.erase(statusLines.begin());
        rect.top -= rect.height;
    }
}

void KitchenState::initIngredientBanners(const sf::Vector2u &winSize,
        std::vector<std::string> &statusLines) {
    sf::FloatRect rect;
    std::string path;
    std::vector<std::string> tokens;
    std::string name;
    int remaining = 0;
    int count = 0;

    rect.left = PERCENTAGE(40, winSize.x);
    rect.top = PERCENTAGE(55, winSize.y);
    rect.width = PERCENTAGE(20, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    for (const std::string &line : statusLines) {
        tokens = split(line, ':');
        name = tokens[0];
        remaining = atoi(tokens[1].c_str());
        path = "./assets/" + name + ".png";
        _textures[name].loadFromFile(path);
        _ingredientBanners[name] = std::make_unique<Banner>(
            rect, _textures[name], name, std::to_string(remaining), _font);
        rect.top += rect.height;
        count++;
        if (count % 3 == 0) {
            rect.left += rect.width;
            rect.top = PERCENTAGE(55, winSize.y);
        }
    }
}

void KitchenState::initBackButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(2, winSize.x);
    rect.top = PERCENTAGE(88, winSize.y);
    rect.width = PERCENTAGE(25, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _backButton = std::make_unique<Button>(rect, "<- Back to reception", _font);
}

void KitchenState::initNextKitchenButton(const sf::Vector2u &winSize) {
    sf::FloatRect rect;

    rect.left = PERCENTAGE(73, winSize.x);
    rect.top = PERCENTAGE(88, winSize.y);
    rect.width = PERCENTAGE(25, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    _nextKitchenButton = std::make_unique<Button>(
        rect, "Next kitchen ->", _font);
}

/* ---------------------------- Member functions ---------------------------- */

void KitchenState::update(const millis_t dt) {
    std::string status;
    std::vector<std::string> lines;

    _timePassed += dt;
    _reception->handleDisconnections();
    if (_activated && _timePassed > 2000) {
        status = _reception->getStatus(_pid);
        if (status.empty()) {
            _announceText.setString("Kitchen closed.");
        } else {
            _timePassed -= 2000;
            lines = split(status, '\n');
            updateNextPizzaText(lines);
            updateCookBanners(lines);
            updateIngredientBanners(lines);
        }
    }
    if (_otherKitchensPids.size() > 0) {
        updateNextKitchenButton();
    }
    updateBackButton();
}

void KitchenState::updateNextPizzaText(std::vector<std::string> &statusLines) {
    sf::Vector2f pos;
    sf::FloatRect bounds;
    sf::Vector2u winSize = this->_win->getSize();
    std::vector<std::string> tokens;

    if (statusLines[0].find(';') == std::string::npos) {
        _nextPizzaText.setString(statusLines[0]);
    } else {
        tokens = split(statusLines[0], ';');
        _nextPizzaText.setString(tokens[0]);
    }
    bounds = _nextPizzaText.getGlobalBounds();
    pos.x = PERCENTAGE(75, winSize.x) - bounds.width / 2;
    pos.y = PERCENTAGE(30, winSize.y);
    _nextPizzaText.setPosition(pos);
    statusLines.erase(statusLines.begin());
}

void KitchenState::updateCookBanners(std::vector<std::string> &statusLines) {
    KitchenSettings settings = _reception->getKitchenSettings();
    std::vector<std::string> tokens;
    int id = 0;
    std::string activity;

    for (size_t i = 0 ; i < settings.nbCooks ; i++) {
        tokens = split(statusLines[0], ':');
        id = atoi(tokens[0].c_str());
        activity = tokens[2];
        _cookBanners[id]->setSubtitle(activity);
        statusLines.erase(statusLines.begin());
    }
}

void KitchenState::updateIngredientBanners(
        std::vector<std::string> &statusLines) {
    std::vector<std::string> tokens;
    std::string name;
    std::string remaining;

    for (const std::string &line : statusLines) {
        tokens = split(line, ':');
        name = tokens[0];
        remaining = tokens[1];
        _ingredientBanners[name]->setSubtitle(remaining);
    }
}

void KitchenState::updateBackButton(void) {
    _backButton->update(sf::Mouse::getPosition(*_win));
    if (_backButton->isPressed()) {
        _states->pop();
    }
}

void KitchenState::updateNextKitchenButton(void) {
    _nextKitchenButton->update(sf::Mouse::getPosition(*_win));
    if (_nextKitchenButton->isPressed()) {
        _states->push(std::make_shared<KitchenState>(
            _stateData, _otherKitchensPids));
    }
}

void KitchenState::render(void) {
    _win->draw(_background);
    _win->draw(_announceText);
    if (_activated) {
        _win->draw(_nextPizzaText);
        for (const auto &[_, banner] : _cookBanners) {
            banner->render(_win);
        }
        for (const auto &[_, banner] : _ingredientBanners) {
            banner->render(_win);
        }
    }
    if (_otherKitchensPids.size() > 0) {
        _nextKitchenButton->render(_win);
    }
    _backButton->render(_win);
}
}  // namespace plz
