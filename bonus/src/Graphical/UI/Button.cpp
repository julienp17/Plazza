/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** Button
*/

#include "Button.hpp"

namespace plz {
/* ----------------------- Constructors / Destructors ----------------------- */

Button::Button(void) {
    auto font = std::make_shared<Font_t>();

    init(sf::FloatRect(0.0f, 0.0f, 0.0f, 0.0f), "", font);
}

Button::Button(const sf::FloatRect &rect, const std::string &text,
        std::shared_ptr<Font_t> font) {
    init(rect, text, font);
}

Button::Button(const sf::FloatRect &rect, const std::string &text,
        std::shared_ptr<Font_t> font, const std::string &texturePath) {
    init(rect, text, font);
    _shape->setFillColor(sf::Color::White);
    _texture = std::make_unique<Texture_t>();
    if (!_texture->loadFromFile(texturePath))
        throw std::runtime_error("Couldn't load " + texturePath);
    _shape->setTexture(_texture.get());
}

/* ------------------------ Initialization functions ------------------------ */

void Button::init(const sf::FloatRect &rect, const std::string &text,
            std::shared_ptr<Font_t> font) {
    _font = font;
    _texture = nullptr;
    _state = IDLE;
    initLooks();
    initShape(rect);
    initText(rect, text, font);
    updateLook();
}

void Button::initShape(const sf::FloatRect &rect) {
    _shape = std::make_unique<sf::RectangleShape>();
    _shape->setFillColor(sf::Color(0, 0, 0, 75));
    _shape->setPosition(rect.left, rect.top);
    _shape->setSize(sf::Vector2f(rect.width, rect.height));
}

void Button::initText(const sf::FloatRect &rect, const std::string &text,
            std::shared_ptr<Font_t> font) {
    _text = std::make_unique<sf::Text>();
    _text->setString(text);
    _text->setFont(*font);
    _text->setFillColor(sf::Color::Black);
    auto bounds = _text->getGlobalBounds();
    _text->setPosition(
        rect.left + (rect.width / 2.f) - bounds.width / 2.f,
        rect.top + (rect.height / 2.f) - bounds.height / 2.f);
}

void Button::initLooks(void) {
    _looks.clear();
    _looks.reserve(3);
    _looks[IDLE]    = {sf::Color::Black, sf::Color::Black,  4.0f};
    _looks[HOVERED] = {sf::Color::Yellow, sf::Color::Yellow, 6.0f};
    _looks[PRESSED] = {sf::Color::Green, sf::Color::Green, 8.0f};
}

/* ---------------------------- Member functions ---------------------------- */

void Button::render(std::shared_ptr<Window_t> window) {
    window->draw(*_shape);
    window->draw(*_text);
}

void Button::update(const sf::Vector2i &mousePosWin) {
    updateState(mousePosWin);
    updateLook();
}

void Button::updateState(const sf::Vector2i &mousePosWin) {
    sf::FloatRect bounds = _shape->getGlobalBounds();

    _state = IDLE;
    if (bounds.contains(mousePosWin.x, mousePosWin.y)) {
        _state = HOVERED;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            _state = PRESSED;
    }
}

void Button::updateLook(void) {
    struct StateLook look = _looks[_state];

    _text->setFillColor(look.textColor);
    _shape->setOutlineThickness(look.outlineThickness);
    _shape->setOutlineColor(look.outlineColor);
}
}  // namespace plz
