/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Pizza
*/

#ifndef INC_PIZZA_HPP_
#define INC_PIZZA_HPP_

#include <string>
#include <memory>
#include "plazza.hpp"

namespace plz {
enum PizzaType {
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

/**
 * @brief Return an enum of PizzaType from a string
 * 
 * @param pType The Pizza type, as a string
 * @return The Pizza type, as an enum
 */
PizzaType getPizzaType(const std::string &pType);

enum PizzaSize {
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

/**
 * @brief Return an enum of PizzaSize from a string
 * 
 * @param pType The Pizza size, as a string
 * @return The Pizza size, as an enum
 */
PizzaSize getPizzaSize(const std::string &pType);

milliseconds_t getPizzaBakeTime(const plz::PizzaType type);

struct Pizza {
    Pizza(const std::string &pType, const std::string &pSize);
    Pizza(const PizzaType pType, const PizzaSize pSize);

    PizzaType type;
    PizzaSize size;
    milliseconds_t timeToBake;
};

typedef std::shared_ptr<struct Pizza> PizzaPtr;

}  // namespace plz

#endif  // INC_PIZZA_HPP_
