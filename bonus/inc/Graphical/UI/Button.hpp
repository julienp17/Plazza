/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** Button
*/

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include <unordered_map>
#include <memory>
#include "AppTypes.hpp"

namespace plz {
class Button {
 public:
    Button(void);
    Button(const sf::FloatRect &rect,
            const std::string &text,
            std::shared_ptr<Font_t> _font);
    Button(const sf::FloatRect &rect,
            const std::string &text,
            std::shared_ptr<Font_t> font,
            const std::string &texturePath);
    virtual ~Button(void) {}

    inline bool isPressed(void) const {
        return _state == PRESSED;
    }

    void update(const sf::Vector2i &mousePosWin);
    void render(std::shared_ptr<Window_t> window);

    std::string getString(void) const {
        return this->_text->getString();
    }

    void setColor(const sf::Color &color) {
        this->_shape->setOutlineColor(color);
        this->_text->setFillColor(color);
    }

 private:
    void init(const sf::FloatRect &rect, const std::string &text,
            std::shared_ptr<Font_t> font);
    void initShape(const sf::FloatRect &rect);
    void initText(const sf::FloatRect &rect, const std::string &text,
            std::shared_ptr<Font_t> font);
    void initLooks(void);
    void updateState(const sf::Vector2i &mousePosWin);
    void updateLook(void);

    std::unique_ptr<sf::RectangleShape> _shape;
    std::unique_ptr<sf::Text> _text;
    std::unique_ptr<Texture_t> _texture;
    std::shared_ptr<Font_t> _font;
    enum ButtonState {
        IDLE,
        HOVERED,
        PRESSED
    } _state;
    struct StateLook {
        sf::Color textColor;
        sf::Color outlineColor;
        double outlineThickness;
    };
    std::unordered_map<enum ButtonState, struct StateLook> _looks;
};
}  // namespace plz

#endif  // INC_BUTTON_HPP_
