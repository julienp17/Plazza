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
#include <chrono>
#include <vector>
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
 * @param type The pizza type, as a string
 * @return The pizza type, as an enum
 */
PizzaType getPizzaType(const std::string &type);

/**
 * @brief Return a string of PizzaType from an enum
 *
 * @param type The pizza type, as an enum
 * @return The pizza type, as a string
 */
std::string getPizzaType(const PizzaType type) noexcept;

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
 * @param size The pizza size, as a string
 * @return The pizza size, as an enum
 */
PizzaSize getPizzaSize(const std::string &size);

/**
 * @brief Return a string of PizzaSize from an enum
 *
 * @param type The pizza size, as an enum
 * @return The pizza size, as a string
 */
std::string getPizzaSize(const PizzaSize size) noexcept;

std::chrono::milliseconds getPizzaBakeTime(const plz::PizzaType type);
std::vector<std::string> getPizzaIngredients(const PizzaType type);

struct Pizza {
    Pizza(const std::string &type, const std::string &size,
        const std::string &id = "");
    Pizza(const PizzaType type, const PizzaSize pSize,
        const std::string &id = "");

    //* The type of the pizza, defined by the PizzaType enum
    PizzaType type;

    //* The size of the pizza, defined by the PizzaSize enum
    PizzaSize size;

    //* The default time for the pizza to bake
    std::chrono::milliseconds timeToBake;

    //* The order id
    std::string id;

    //* The ingredients required to make the pizza
    std::vector<std::string> ingredients;
};

}  // namespace plz

std::ostream &operator<<(std::ostream &out, const plz::PizzaType type);
std::ostream &operator<<(std::ostream &out, const plz::PizzaSize size);
std::ostream &operator<<(std::ostream &out, const plz::Pizza &pizza);

#endif  // INC_PIZZA_HPP_
