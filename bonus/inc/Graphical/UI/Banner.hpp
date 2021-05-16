/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** Banner
*/

#ifndef COOKID_HPP_
#define COOKID_HPP_

#include "AppTypes.hpp"
#include "Cook.hpp"

namespace plz {
class Banner {
 public:
    Banner(const sf::FloatRect &rect,
            const Texture_t &texture,
            const std::string &title,
            const std::string &subtitle,
            std::shared_ptr<Font_t> font);
    virtual ~Banner(void) {}

    void render(std::shared_ptr<Window_t> window);

    Texture_t getPictureTex(void) const {
        return _pictureTex;
    }
    std::string getTitle(void) const {
        return _title.getString();
    }
    std::string getSubtitle(void) const {
        return _subtitle.getString();
    }

    void setPictureTex(const Texture_t &texture) {
        _pictureTex = texture;
    }
    void setTitle(const std::string &title) {
        _title.setString(title);
    }
    void setSubtitle(const std::string &subtitle) {
        _subtitle.setString(subtitle);
    }

 private:
    void initBackground(const sf::FloatRect &rect);
    void initPicture(const sf::FloatRect &rect, const Texture_t &texture);
    void initTitle(const sf::FloatRect &rect, const std::string &titleStr);
    void initSubtitle(const sf::FloatRect &rect,const std::string &subTitleStr);

    sf::RectangleShape _background;
    sf::RectangleShape _picture;
    Texture_t _pictureTex;
    sf::Text _title;
    sf::Text _subtitle;
    std::shared_ptr<Font_t> _font;
};
}  // namespace plz

#endif /* !COOKID_HPP_ */
