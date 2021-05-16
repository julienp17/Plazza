/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** MenuState
*/

#include "MenuState.hpp"
#include "ReceptionState.hpp"
#include "SettingsState.hpp"

namespace plz {
/* ----------------------------- Ctors / Dtors ------------------------------ */

MenuState::MenuState(const struct StateData &stateData) : AState(stateData) {
    sf::FloatRect rect;
    sf::Vector2u winSize = stateData.window->getSize();
    std::vector<std::string> buttonsNames = {"Start", "Settings", "Quit"};

    rect.left = PERCENTAGE(37, winSize.x);
    rect.top = PERCENTAGE(50, winSize.y);
    rect.width = PERCENTAGE(25, winSize.x);
    rect.height = PERCENTAGE(10, winSize.y);
    this->_textures["background"].loadFromFile("./assets/menu_background.png");
    for (const std::string &name : buttonsNames) {
        this->_buttons[name] = std::make_unique<Button>(rect, name, _font);
        rect.top += PERCENTAGE(15, winSize.y);
    }
}

/* ---------------------------- Member functions ---------------------------- */

void MenuState::update(const millis_t dt) {
    (void)dt;
    for (auto & [_, button] : this->_buttons)
        button->update(sf::Mouse::getPosition(*this->_win));
    if (_buttons["Start"]->isPressed())
        this->_states->push(std::make_shared<ReceptionState>(_stateData));
    if (_buttons["Settings"]->isPressed())
        this->_states->push(std::make_shared<SettingsState>(_stateData));
    if (_buttons["Quit"]->isPressed())
        this->_win->close();
}

void MenuState::render(void) {
    sf::RectangleShape background;

    background.setTexture(&this->_textures["background"]);
    background.setSize(static_cast<sf::Vector2f>(_win->getSize()));
    _win->draw(background);
    for (auto & [_, button] : this->_buttons)
        button->render(_win);
}
}  // namespace plz
