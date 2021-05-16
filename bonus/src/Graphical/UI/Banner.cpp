/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** Banner
*/

#include "Banner.hpp"

namespace plz {
Banner::Banner(const sf::FloatRect &rect,
            const Texture_t &texture,
            const std::string &title,
            const std::string &subtitle,
            std::shared_ptr<Font_t> font) {
    _font = font;
    initBackground(rect);
    initPicture(rect, texture);
    initTitle(rect, title);
    initSubtitle(rect, subtitle);
}

void Banner::initBackground(const sf::FloatRect &rect) {
    _background.setPosition(rect.left, rect.top);
    _background.setSize(sf::Vector2f(rect.width, rect.height));
    _background.setFillColor(sf::Color(0, 0, 0, 75));
    _background.setOutlineColor(sf::Color::Black);
    _background.setOutlineThickness(2.0f);
}

void Banner::initPicture(const sf::FloatRect &rect, const Texture_t &texture) {
    float xMargin = PERCENTAGE(10, rect.width / 3);
    float yMargin = PERCENTAGE(10, rect.height);

    _pictureTex = texture;
    _picture.setTexture(&_pictureTex);
    _picture.setPosition(rect.left + xMargin, rect.top + yMargin);
    _picture.setSize(sf::Vector2f(rect.width / 3, rect.height - yMargin * 2));
}

void Banner::initTitle(const sf::FloatRect &rect, const std::string &titleStr) {
    sf::FloatRect titleBounds;

    _title.setFillColor(sf::Color::Black);
    _title.setFont(*_font);
    _title.setString(titleStr);
    _title.setPosition(
        rect.left + rect.width / 3 + rect.width / 15,
        rect.top);
}

void Banner::initSubtitle(const sf::FloatRect &rect,
        const std::string &subtitleStr) {
    sf::FloatRect titleBounds;

    _subtitle.setFillColor(sf::Color::Black);
    _subtitle.setFont(*_font);
    _subtitle.setString(subtitleStr);
    titleBounds = _title.getGlobalBounds();
    _subtitle.setPosition(
        rect.left + rect.width / 3 + rect.width / 15,
        rect.top + titleBounds.height + titleBounds.height / 2);
}

void Banner::render(std::shared_ptr<Window_t> window) {
    window->draw(_background);
    window->draw(_picture);
    window->draw(_title);
    window->draw(_subtitle);
}

}  // namespace plz
