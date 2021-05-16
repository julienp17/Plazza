/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** App
*/

#include <iostream>
#include "App.hpp"
#include "MenuState.hpp"

namespace plz {
/* ----------------------------- Ctors / Dtors ------------------------------ */

App::App(void) {
    this->initWindow();
    this->_reception = std::make_shared<Reception>();
    this->_states = std::make_shared<std::stack<std::shared_ptr<AState>>>();
    this->initStates();
    this->_clock = std::make_unique<Clock_t>();
}

/* ------------------------------ Initializers ------------------------------ */

void App::initWindow(void) {
    sf::VideoMode mode(1280, 720);
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2i pos;
    std::string title = "Plazza";

    this->_win = std::make_shared<Window_t>(mode, title, sf::Style::Close);
    pos.x = desktop.width / 2 - mode.width / 2;
    pos.y = desktop.height / 2 - mode.height / 2;
    this->_win->setPosition(pos);
}

void App::initStates(void) {
    auto font = std::make_shared<Font_t>();

    if (!font->loadFromFile("./assets/pizzaria.ttf"))
        throw std::runtime_error("Couldn't load default font");
    struct StateData data = {
        this->_win, font, this->_reception, this->_states};
    this->_states->push(std::make_shared<MenuState>(data));
}

/* ---------------------------- Member functions ---------------------------- */

void App::run(void) {
    while (this->_win->isOpen()) {
        this->update();
        this->render();
    }
}

void App::update(void) {
    millis_t dt = this->_clock->restart().asMilliseconds();

    this->handleEvents();
    if (!this->_states->empty())
        this->_states->top()->update(dt);
}

void App::render(void) {
    this->_win->clear();
    if (!this->_states->empty())
        this->_states->top()->render();
    this->_win->display();
}

void App::handleEvents(void) {
    sf::Event event;

    while (this->_win->pollEvent(event)) {
        if (event.type == sf::Event::Closed || (
            event.type == sf::Event::KeyPressed
            && event.key.code == sf::Keyboard::Escape))
            this->_win->close();
    }
}
}  // namespace plz
