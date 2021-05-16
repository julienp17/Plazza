/*
** EPITECH PROJECT, 2021
** bonus [WSL: Ubuntu-20.04]
** File description:
** AppTypes
*/

#ifndef INC_GAMETYPES_HPP_
#define INC_GAMETYPES_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define PERCENTAGE(coord, size) ((coord * size) / 100)

namespace plz {
using Window_t = sf::RenderWindow;
using Texture_t = sf::Texture;
using Font_t = sf::Font;
using Clock_t = sf::Clock;
using millis_t = int;
}

#endif  // INC_GAMETYPES_HPP_
