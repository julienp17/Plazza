/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** Counter
*/

#ifndef INC_COUNTER_HPP_
#define INC_COUNTER_HPP_

#include "AppTypes.hpp"
#include "Button.hpp"

namespace plz {
template <typename T>
class Counter {
 public:
    Counter(const sf::FloatRect &rect,
            std::shared_ptr<Font_t> font,
            const std::string &str = "") {
        sf::FloatRect tmpRect = rect;

        this->_decrementButton = std::make_shared<Button>(rect, "-", font);
        tmpRect.left = rect.left + rect.width * 3;
        this->_incrementButton = std::make_shared<Button>(tmpRect, "+", font);
        _counter = 0;
        _step = 1;
        _font = font;
        _title.setFont(*font);
        _title.setFillColor(sf::Color::Black);
        _title.setString(str);
        _counterText.setFont(*font);
        _counterText.setFillColor(sf::Color::Black);
        _counterText.setString(std::to_string(_counter));
        auto bounds = _counterText.getGlobalBounds();
        _counterText.setPosition(
            rect.left + (rect.width * 2) - bounds.width / 2.f,
            rect.top + (rect.height / 2.f) - bounds.height / 2.f);
        _title.setPosition(rect.left, rect.top - bounds.height * 2.f);
    }

    virtual ~Counter(void) {}

    void update(const sf::Vector2i &mousePosWin) {
        _decrementButton->update(mousePosWin);
        if (_decrementButton->isPressed()) {
            _decrementButton->setColor(sf::Color::Green);
            _counter -= _step;
            _counterText.setString(std::to_string(_counter));
        }
        _incrementButton->update(mousePosWin);
        if (_incrementButton->isPressed()) {
            _incrementButton->setColor(sf::Color::Green);
            _counter += _step;
            _counterText.setString(std::to_string(_counter));
        }
    }

    void render(std::shared_ptr<Window_t> window) {
        window->draw(_title);
        _decrementButton->render(window);
        window->draw(_counterText);
        _incrementButton->render(window);
    }

    T getCount(void) const {
        return _counter;
    }
    void setCount(const T count) {
        _counter = count;
        _counterText.setString(std::to_string(_counter));
    }

    T getStep(void) const {
        return _step;
    }
    void setStep(const T step) {
        _step = step;
    }

 private:
    T _counter;
    T _step;
    sf::Text _title;
    sf::Text _counterText;
    std::shared_ptr<Font_t> _font;
    std::shared_ptr<Button> _decrementButton;
    std::shared_ptr<Button> _incrementButton;
};
}  // namespace plz

#endif  // INC_COUNTER_HPP_
